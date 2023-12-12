#include <curses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "ui.h"
#include "music-functions.h"

Note notes[MAX_NUM_DURATIONS];
int score = 0;
int combo = 0;
int current_note_index = 0;
int sequence_length = 0; 
int offset = 0; 

pthread_mutex_t mutex;
char notes_letters[8] = {'A','S','D', 'F', 'H', 'J','K','L'};

/** initialize the notes by inputing the notes and corresponding duration and calculate when and where should a note drop on the ui
 *
 * \param note_sequence A string of music notes
 * 
 * \param durations The time each note will last
 *
 */
void init_notes(int * note_sequence, double* durations) {
    time_t currentTime;
    time(&currentTime);
    for (int i = 0; i < sequence_length; i++) {
        int index1 = note_sequence[i];
        notes[i].letter = notes_letters[index1];
        // Calculte the x coordinates
        notes[i].x = note_sequence[i] * (WIDTH / NUM_NOTES) + (WIDTH / NUM_NOTES - 1) / 2;
        // set active is false
        notes[i].active = false;
        // Initial y postion is at 1
        notes[i].y = 1;
        // If the note is the firt note
        if (i == 0){
        // The droptime for first note is current time plus offset time + duration
        notes[i].dropTime = currentTime + offset;
        } else{
        // Drop time for other notes is the drop time of the previous note plus the duration of the previous note
        notes[i].dropTime = notes[i-1].dropTime + durations[i-1];
        }
        
    }
}

// check if a notes is suppose to drop at certain time in the ui
void generate_note() {
    
    time_t currentTime2;
    // Current time
    time(&currentTime2);
    // if the current index is less the number of notes and we are at or passed the drop time
    if (current_note_index < sequence_length && difftime(currentTime2, notes[current_note_index].dropTime) >= 0) {
    // set the note to be active meaning the note is reading to drop
    notes[current_note_index].active = true;
    current_note_index++;
    }
}

// move te notes downward
void move_notes() {
    // Loop throught all the avaalable notes
    for (int i = 0; i < sequence_length; i++) {
        // If the note  is active
        if (notes[i].active) {
            // Move y corrdinate downward by incrementing
            notes[i].y++;
            //If the note reach bottom
            if (notes[i].y > HEIGHT-1) {
                // deactive the note
                notes[i].active = false;
                // set combo to 0 since the player missed the note
                combo = 0;
            }
        }
    }
}

// Draw the board of ui
void draw() {
    clear();
    // draw the scoring line
    mvhline(HEIGHT - 2, 0, '-', WIDTH);
    mvhline(HEIGHT - 6, 0, '-', WIDTH);
    // draw the reference letters at the top of the row
    for (int i = 0; i < NUM_NOTES; i++) {
        int location = i * (WIDTH / NUM_NOTES) + (WIDTH / NUM_NOTES - 1) / 2;
        mvaddch(0, location, notes_letters[i]);
    }
    // draw the active notes
    for (int i = 0; i < sequence_length; i++) {
        if (notes[i].active) {
        mvaddch(notes[i].y, notes[i].x, notes[i].letter);
        }
    }
    // print the score and combo at the bottom of the board
    mvprintw(HEIGHT-1, 0, "Score: %d", score);
    mvprintw(HEIGHT, 0, "Combo: %d", combo);
    // refresh the board
    refresh();
}

/** integrate generate note and move note together and use mutex to prevent race condition
 *
 * \param args remnant of thread
 * 
 */
void *note_movement(void *args) {
    while (true) {
        pthread_mutex_lock(&mutex);
        generate_note();
        move_notes();
        pthread_mutex_unlock(&mutex);
        usleep(DROP_INTERVAL);
    }
    return NULL;
}

/** Write the final score to a file
 *
 * \param finalScore final score of a game
 * 
 */
void record_score(int finalScore) {
    FILE *file = fopen("scores.txt", "a"); // Open scores.txt in append mode
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "Score: %d\n", finalScore); // Write the score to the file
    fclose(file); // Close the file
}

/** Read the score file
 *
 * \param scores[] scores array
 * 
 * \param max_scores maximum number of scores
 */
int read_scores(int scores[], int max_scores) {
    FILE *file = fopen("scores.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    int score, count = 0;
    while (count < max_scores && fscanf(file, "Score: %d\n", &score) == 1) {
        scores[count++] = score;
    }

    fclose(file);
    return count; // Return the number of scores read
}

/** find the max number of scores
 *
 * \param scores[] scores array
 * 
 * \param max_scores number of scores
 */
void sort_scores(int scores[], int count) {
    // Loop through the scores array to sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (scores[j] < scores[j + 1]) {
                int temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }
}

/** Print the top 5 scores
 *
 * \param scores[] scores array
 * 
 * \param max_scores number of scores
 */
void print_top_scores(int scores[], int count) {
    printf("Top Scores:\n");
    for (int i = 0; i < count && i < 5; i++) {
        printf("%d. %d\n", i + 1, scores[i]);
    }
}

//check the input of the user to calculate score and combo
void check_input() {
    int ch = getch();  // Get a character from the user
    if (ch != ERR) {  // If a key was pressed
        ch = toupper(ch);  // Convert to uppercase
        
        pthread_mutex_lock(&mutex);  // Lock the mutex
        for (int i = 0; i < sequence_length; i++) {  // Loop through the notes
            if (notes[i].active && ch == notes[i].letter) {  // If the note is active and the key matches
                // Check if the note is within the perfect (middle) of scoring area
                  if ((HEIGHT - 2 - notes[i].y) > HIT_MARGIN && (notes[i].y - HEIGHT +6) > HIT_MARGIN) {  
                    //Increment combo
                    combo++;
                    // If combo is greater than 10
                    if(combo >= 10){
                        score = score+100*1.5; // Increase score with a multiplier 1.5
                        notes[i].active = false;// Deactivate the note
                     // If combo is greater than 20
                    }else if(combo >= 20){
                        score = score+100*2; // Increase score with a multiplier 2
                        notes[i].active = false;
                    }
                    else if (combo<10){
                    score = score+100;   // Increase score with no multiplier
                    notes[i].active = false;  
                    }
                    
                    
                // Check if the note is barely hitting the scoring area
                } else if((HEIGHT - 2 - notes[i].y) == 0) {
                    // set combo to 0
                    combo = 0;
                    score = score+10;  // Increase score
                    notes[i].active = false;  // Deactivate the note
                // Check if the note is barely hitting the scoring area
                }else if((HEIGHT - 6 - notes[i].y) == 0) {
                    combo = 0;
                    score = score+10;  // Increase score
                    notes[i].active = false;  // Deactivate the note
                // Check if the note is within the scoring area but not perfectly in the middle
                }else if((HEIGHT - 6 - notes[i].y) < 0 && abs(HEIGHT - 6 - notes[i].y) <=HIT_MARGIN) {
                    
                    combo++;
                    if(combo >= 10){
                        score = score+50*1.5;
                        notes[i].active = false;
                    }else if(combo >= 20){
                        score = score+50*2;
                        notes[i].active = false;
                    }
                    else if (combo<10){
                    score = score+50;  // Increase score
                    notes[i].active = false;  // Deactivate the note
                    }
                // Check if the note is within the scoring area but not perfectly in the middle
                }else if((HEIGHT - 2 - notes[i].y) > 0 && abs(HEIGHT - 2 - notes[i].y) <=HIT_MARGIN) {
                   
                    combo++;
                    if(combo >= 10){
                        score = score+50*1.5;
                        notes[i].active = false;
                    }else if(combo >= 20){
                        score = score+50*2;
                        notes[i].active = false;
                    }
                    else if (combo<10){
                    score = score+50;  // Increase score
                    notes[i].active = false;  // Deactivate the note
                    }
                // Check if the note is outside the scoreing area
                }else if((HEIGHT - 6 - notes[i].y) > 0 && abs(HEIGHT - 6 - notes[i].y) <=HIT_MARGIN) {
                    score = score+10;  // Increase score
                    notes[i].active = false;  // Deactivate the note
                    combo = 0;
                // Check if the note is outside the scoreing area
                }else if((HEIGHT - 2 - notes[i].y) < 0 && abs(HEIGHT - 2 - notes[i].y) <=HIT_MARGIN) {
                    score = score+10;  // Increase score
                    notes[i].active = false;  // Deactivate the note
                    combo = 0;
                    
                }
            }
        }
        pthread_mutex_unlock(&mutex);  // Unlock the mutex
    }
}


/**
 * A function that Check if the number of notes that are deactived
 * 
 * \return the number of deactived notes
*/
bool check_active(){
    bool active =  true; 
    int count = 0;
    // Loop through all the notes to see how many note are deactive
    for(int i = 0; i < sequence_length; i++){
         if (notes[i].active == false){
            count ++;
         }   
        }
    
    if(count == sequence_length){
        active = false;
    }
    // Return how many notes are deactived
    return active; 
}

// A function that will print the high-scores found in the scores.txt file
void print_scores(){
    printf("Your Score: %d\n", score); 
    int scores[MAX_SCORES];
    int count = read_scores(scores, MAX_SCORES);
    sort_scores(scores, count);
    if(scores[0] == score){
        printf("\n***NEW HIGH SCORE***\n\n"); 
    }
    print_top_scores(scores, count);
}


/** integrate everthing together and run the ui
 *
 * \param args remnant of thread
 * 
 */
void * run_game(void* args) {
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);

    pthread_t movement_thread;
    thread_struct_t input_args = *((thread_struct_t *) args);

    sequence_length = input_args.song_total_duration;  

    pthread_mutex_init(&mutex, NULL);

    init_notes(input_args.index, input_args.duration);

    // We need to sync up the music and game together
    //sleep(8); 

    pthread_create(&movement_thread, NULL, note_movement, &input_args);

    // Looping while true
    while (true) {
        // draw the board
        draw();
        // check inpu
        check_input();
        if (current_note_index == sequence_length && !check_active()) {  // End game condition
            record_score(score);
            draw();
            break;
        }

        usleep(50000); // Refresh rate
    }

    pthread_mutex_lock(&mutex);
    pthread_cancel(movement_thread);
    pthread_mutex_unlock(&mutex);

    pthread_join(movement_thread, NULL);
    pthread_mutex_destroy(&mutex);

    mvprintw(HEIGHT + 2, 0, "Game Over! Press any key to exit.");
    nodelay(stdscr, FALSE);
    getch();

    endwin();
    print_scores(); 

    return NULL; 
}