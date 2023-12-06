#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "music-functions.h"
#include "ui.h"

int main() { 

    // initializing frequencies and durations arrays 
    double frequencies[MAX_NUM_FREQUENCIES]; 
    double durations[MAX_NUM_DURATIONS]; 
    //NOTE: Letters will be represented as there locations
    // in the keyboard (A=0, S=1, D=2, F=3, H=4, J=5, K =6, L=7)
    int letters[MAX_NUM_DURATIONS]; 
    int num_durations; 

    // generating music based on user input 
    // storing values in frequencies and durations arrays 
    num_durations = inputs(frequencies, letters, durations); 

    // for(int i = 0; i < MAX_NUM_DURATIONS; i++){
    //     printf("Keyboard Characters: %c\n", letters[i]); 
    // }

    // Since this will only have ever like 2 threads, I think
    // it would be easier to do this as a process 

    pthread_t game_thread;

    thread_struct_t thread_args; 

    thread_args.duration = durations; 
    thread_args.index = letters; 
    thread_args.song_total_duration = num_durations; 

    pthread_create(&game_thread, NULL, run_game, &thread_args); 

    //sleep(10); 
    // using arrays to create output 
    playMusic(frequencies, durations, MAX_NUM_FREQUENCIES, MAX_NUM_DURATIONS, num_durations); 
    
    pthread_join(game_thread, NULL); 

    // Any other artifacts we make, we will want to clean up
    // cleanup_mem(); 

    return 0; 
}

