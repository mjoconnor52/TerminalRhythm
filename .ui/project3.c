#include <curses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define WIDTH 50
#define HEIGHT 20
#define DROP_INTERVAL 500000  // Microseconds
#define HIT_MARGIN 1          // Second
#define NUM_NOTES 8
#define MAX_SCORES 1000

typedef struct {
    char letter;
    int x, y;
    bool active;
    time_t dropTime;
    int combo;
} Note;

Note notes[NUM_NOTES];
int score = 0;
int combo = 0;
pthread_mutex_t mutex;

char notes_letters[8] = {'A','S','D', 'F', 'H', 'J','K','L'};

void init_notes() {
    for (int i = 0; i < NUM_NOTES; i++) {
        notes[i].letter = notes_letters[i];
        notes[i].x = i * (WIDTH / NUM_NOTES) + (WIDTH / NUM_NOTES - 1) / 2;
        notes[i].y = 0;
        notes[i].active = false;
        notes[i].dropTime = 0;
    }
}

void generate_note() {
    int note_index = rand() % NUM_NOTES;
    if (!notes[note_index].active) {
        notes[note_index].active = true;
        notes[note_index].y = 1;
        time(&notes[note_index].dropTime);
    }
}

void move_notes() {
    for (int i = 0; i < NUM_NOTES; i++) {
        if (notes[i].active) {
            notes[i].y++;
            if (notes[i].y > HEIGHT-1) {
                notes[i].active = false;
            }
        }
    }
}

void draw() {
    clear();
    mvhline(HEIGHT - 2, 0, '-', WIDTH);
    mvhline(HEIGHT - 6, 0, '-', WIDTH);
    for (int i = 0; i < NUM_NOTES; i++) {
        mvaddch(0, notes[i].x, notes_letters[i]);
    }
    for (int i = 0; i < NUM_NOTES; i++) {
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
        for (int i = 0; i < NUM_NOTES; i++) {  // Loop through the notes
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

int main() {
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);

    pthread_t movement_thread;
    pthread_mutex_init(&mutex, NULL);

    init_notes();

    pthread_create(&movement_thread, NULL, note_movement, NULL);

    while (true) {
        draw();
        check_input();

        if (score >= 2000) {  // End game condition
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
    return 0;
}