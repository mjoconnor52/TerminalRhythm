#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <string.h>

typedef struct {
    char notes[6]; // Stores the current notes for A-F
    int score;
    pthread_mutex_t lock;
} GameState;

void* spawn_notes(void* arg) {
    GameState* state = (GameState*)arg;
    while (1) {
        int noteIndex = rand() % 6;
        pthread_mutex_lock(&state->lock);
        state->notes[noteIndex] = 'A' + noteIndex;
        pthread_mutex_unlock(&state->lock);
        sleep(1); // Spawn a note every second
    }
    return NULL;
}

void* handle_input(void* arg) {
    GameState* state = (GameState*)arg;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt); // Get current terminal settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable buffering and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (1) {
        char ch = getchar();
        pthread_mutex_lock(&state->lock);
        for (int i = 0; i < 6; i++) {
            if (state->notes[i] == ch) {
                state->score++;
                state->notes[i] = ' ';
                break;
            }
        }
        pthread_mutex_unlock(&state->lock);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore terminal settings
    return NULL;
}

int main() {
    GameState state;
    memset(&state, 0, sizeof(state));
    pthread_mutex_init(&state.lock, NULL);

    pthread_t spawn_thread, input_thread;
    srand(time(NULL));

    pthread_create(&spawn_thread, NULL, spawn_notes, &state);
    pthread_create(&input_thread, NULL, handle_input, &state);

    while (1) {
        pthread_mutex_lock(&state.lock);
        // Clear screen and redraw UI
        system("clear");
        printf("A B C D E F\n");
        for (int i = 0; i < 6; i++) {
            printf("%c ", state.notes[i]);
            state.notes[i] = ' '; // Clear the note after displaying
        }
        printf("\nScore: %d\n", state.score);
        pthread_mutex_unlock(&state.lock);
        usleep(100000); // Update UI every 0.1 seconds
    }

    pthread_join(spawn_thread, NULL);
    pthread_join(input_thread, NULL);
    pthread_mutex_destroy(&state.lock);

    return 0;
}