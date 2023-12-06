#include <curses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include "ui.h"

Note notes[AMOUNT_NOTES];
int score = 0;
int combo = 0;
int current_note_index = 0;
int sequence_length = 0; 
int offset = 0; 

pthread_mutex_t mutex;
char notes_letters[8] = {'A','S','D', 'F', 'H', 'J','K','L'};

void init_notes(int * note_sequence, double* durations) {
    time_t currentTime;
    time(&currentTime);
    for (int i = 0; i < sequence_length; i++) {
        int index1 = note_sequence[i];
        notes[i].letter = notes_letters[index1];
        notes[i].x = note_sequence[i] * (WIDTH / NUM_NOTES) + (WIDTH / NUM_NOTES - 1) / 2;
        notes[i].active = false;
        notes[i].y = 1;
        if (i == 0){
        notes[i].dropTime = currentTime + offset + durations[i];
        } else{
        notes[i].dropTime = notes[i-1].dropTime + durations[i];
        }
        
    }
}

void generate_note() {
    time_t currentTime2;
    time(&currentTime2);
   
    if (current_note_index < sequence_length && difftime(currentTime2, notes[current_note_index].dropTime) >= 0) {
    notes[current_note_index].active = true;
    current_note_index++;
    }
}

void move_notes() {
    for (int i = 0; i < sequence_length; i++) {
        if (notes[i].active) {
            notes[i].y++;
            if (notes[i].y > HEIGHT-1) {
                notes[i].active = false;
                combo = 0;
            }
        }
    }
}

void draw() {
    clear();
    mvhline(HEIGHT - 2, 0, '-', WIDTH);
    mvhline(HEIGHT - 6, 0, '-', WIDTH);
    for (int i = 0; i < NUM_NOTES; i++) {
        int location = i * (WIDTH / NUM_NOTES) + (WIDTH / NUM_NOTES - 1) / 2;
        mvaddch(0, location, notes_letters[i]);
    }
    for (int i = 0; i < sequence_length; i++) {
        if (notes[i].active) {
        mvaddch(notes[i].y, notes[i].x, notes[i].letter);
        }
    }
    
    mvprintw(HEIGHT-1, 0, "Score: %d", score);
    mvprintw(HEIGHT, 0, "Combo: %d", combo);
    refresh();
}

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


void record_score(int finalScore) {
    FILE *file = fopen("scores.txt", "a"); // Open scores.txt in append mode
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "Score: %d\n", finalScore); // Write the score to the file
    fclose(file); // Close the file
}

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
void sort_scores(int scores[], int count) {
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

void print_top_scores(int scores[], int count) {
    printf("Top Scores:\n");
    for (int i = 0; i < count && i < 5; i++) {
        printf("%d. %d\n", i + 1, scores[i]);
    }
}
// void check_input() {
//     int ch = getch();
//     if (ch != ERR) {
//         ch = toupper(ch);
//         time_t currentTime;
//         time(&currentTime);
        
//         pthread_mutex_lock(&mutex);
//         for (int i = 0; i < NUM_NOTES; i++) {
//             if (notes[i].active && ch == notes[i].letter) {
//                 double diff = difftime(currentTime, notes[i].dropTime);
//                 if (abs(diff) <= HIT_MARGIN) {
//                     score++;
//                     notes[i].active = false;
//                 }
//             }
//         }
//         pthread_mutex_unlock(&mutex);
//     }
// }


void check_input() {
    int ch = getch();  // Get a character from the user
    if (ch != ERR) {  // If a key was pressed
        ch = toupper(ch);  // Convert to uppercase
        
        pthread_mutex_lock(&mutex);  // Lock the mutex
        for (int i = 0; i < sequence_length; i++) {  // Loop through the notes
            if (notes[i].active && ch == notes[i].letter) {  // If the note is active and the key matches
                // Check if the note is within the bottom line HIT_MARGIN
                  if ((HEIGHT - 2 - notes[i].y) > HIT_MARGIN && (notes[i].y - HEIGHT +6) > HIT_MARGIN) {  
                    combo++;
                    if(combo >= 10){
                        score = score+100*1.5;
                        notes[i].active = false;
                    }else if(combo >= 20){
                        score = score+100*2;
                        notes[i].active = false;
                    }
                    else if (combo<10){
                    score = score+100;  // Increase score
                    notes[i].active = false;  // Deactivate the note
                    }
                    
                    
                    
                } else if((HEIGHT - 2 - notes[i].y) == 0) {
                    combo = 0;
                    score = score+10;  // Increase score
                    notes[i].active = false;  // Deactivate the note
                }else if((HEIGHT - 6 - notes[i].y) == 0) {
                    combo = 0;
                    score = score+10;  // Increase score
                    notes[i].active = false;  // Deactivate the note

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
                    
                }else if((HEIGHT - 6 - notes[i].y) > 0 && abs(HEIGHT - 6 - notes[i].y) <=HIT_MARGIN) {
                    score = score+10;  // Increase score
                    notes[i].active = false;  // Deactivate the note
                    combo = 0;
                    
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

bool check_active(){
    bool active =  true; 
    int count = 0;
    for(int i = 0; i < sequence_length; i++){
         if (notes[i].active == false){
            count ++;
         }   
        }
    
    if(count == sequence_length){
        active = false;
    }
    return active; 
}

// 
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

    while (true) {
        draw();
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
    int scores[MAX_SCORES];
    int count = read_scores(scores, MAX_SCORES);
    sort_scores(scores, count);
    print_top_scores(scores, count);

    return NULL; 
}