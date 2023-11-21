#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#define WIDTH 50
#define HEIGHT 20
#define DROP_INTERVAL 2  // Seconds
#define HIT_MARGIN 1     // Second
#define NUM_NOTES 6      // Number of notes A-F

typedef struct {
    char letter;
    int x, y;
    bool active;
} Note;

Note notes[NUM_NOTES];
int score = 0;

// Initialize the notes
void init_notes() {
    for (int i = 0; i < NUM_NOTES; i++) {
        notes[i].letter = 'A' + i;
        notes[i].x = i * (WIDTH / NUM_NOTES) + (WIDTH / NUM_NOTES - 1) / 2;
        notes[i].y = 0;
        notes[i].active = false;
    }
}

// Function to generate a random note
void generate_note() {
    int note_index = rand() % NUM_NOTES;
    if (!notes[note_index].active) {
        notes[note_index].active = true;
        notes[note_index].y = 1;
    }
}

// Draw the game board and notes
void draw() {
    clear();

    // Draw A-F at the top row
    for (int i = 0; i < NUM_NOTES; i++) {
        mvaddch(0, notes[i].x, 'A' + i);
    }

    for (int i = 0; i < NUM_NOTES; i++) {
        if (notes[i].active) {
            mvaddch(notes[i].y, notes[i].x, notes[i].letter);
        }
    }

    // Draw bottom line
    mvhline(HEIGHT - 1, 0, '-', WIDTH);

    // Draw score
    mvprintw(HEIGHT, 0, "Score: %d", score);

    refresh();
}

// Update the positions of the notes
void update_notes() {
    for (int i = 0; i < NUM_NOTES; i++) {
        if (notes[i].active) {
            notes[i].y++;
            if (notes[i].y >= HEIGHT - 1) {
                notes[i].active = false;
            }
        }
    }
}

// Check for keypresses and update score
void check_input() {
    int ch = getch();
    if (ch != ERR) {
        ch = toupper(ch);  // Convert to uppercase for comparison
        for (int i = 0; i < NUM_NOTES; i++) {
            if (notes[i].active && notes[i].y == HEIGHT - 1 && ch == notes[i].letter) {
                score++;
                notes[i].active = false;
            }
        }
    }
}

int main() {
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);

    init_notes();

    while (true) {
        generate_note();
        draw();
        update_notes();
        check_input();

        usleep(DROP_INTERVAL * 1000000);

        if (score >= 10) {  // End game condition
            break;
        }
    }

    mvprintw(HEIGHT + 2, 0, "Game Over! Press any key to exit.");
    nodelay(stdscr, FALSE);
    getch();

    endwin();
    return 0;
}