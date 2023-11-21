#include <curses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define WIDTH 50
#define HEIGHT 20
#define DROP_INTERVAL 100000  // Microseconds
#define HIT_MARGIN 1          // Second
#define NUM_NOTES 6

typedef struct {
    char letter;
    int x, y;
    bool active;
    time_t dropTime;
} Note;

Note notes[NUM_NOTES];
int score = 0;
pthread_mutex_t mutex;

void init_notes() {
    for (int i = 0; i < NUM_NOTES; i++) {
        notes[i].letter = 'A' + i;
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
            if (notes[i].y > HEIGHT) {
                notes[i].active = false;
            }
        }
    }
}

void draw() {
    clear();
    for (int i = 0; i < NUM_NOTES; i++) {
        mvaddch(0, notes[i].x, 'A' + i);
    }
    for (int i = 0; i < NUM_NOTES; i++) {
        if (notes[i].active) {
            mvaddch(notes[i].y, notes[i].x, notes[i].letter);
        }
    }
    mvhline(HEIGHT - 1, 0, '-', WIDTH);
    mvprintw(HEIGHT, 0, "Score: %d", score);
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
                if (abs(HEIGHT - 1 - notes[i].y) <= HIT_MARGIN) {  
                    score++;  // Increase score
                    notes[i].active = false;  // Deactivate the note
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

        if (score >= 10) {  // End game condition
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
    return 0;
}