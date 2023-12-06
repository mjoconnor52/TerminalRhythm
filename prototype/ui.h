#pragma once

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
#define NUM_NOTES 8
#define MAX_SCORES 1000
#define AMOUNT_NOTES 100

typedef struct {
    char letter;
    int x, y;
    bool active;
    time_t dropTime;
    int combo;
} Note;

typedef struct thread_struct{ 
    double *duration; 
    int * index;
    int song_total_duration; 
} thread_struct_t;

void init_notes();

void generate_note(); 

void move_notes(); 

void draw(); 

void record_score(int finalScore); 

int read_scores(int scores[], int max_scores); 

void sort_scores(int scores[], int count); 

void print_top_scores(int scores[], int count); 

void check_input(); 

void * run_game(void * args); 