#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "music-functions.h"
#include "ui.h"

void play_game(){
    // initializing frequencies and durations arrays 
    double frequencies[MAX_NUM_FREQUENCIES]; 
    double durations[MAX_NUM_DURATIONS]; 
    //NOTE: Letters will be represented as there locations
    // in the keyboard (A=0, S=1, D=2, F=3, H=4, J=5, K =6, L=7)
    int letters[MAX_NUM_DURATIONS]; 
    int num_durations; 

    // generating music based on user input 
    // storing values in frequencies and durations arrays 
    num_durations = create_random_song(frequencies, letters, durations); 

    // Since this will only have ever like 2 threads, I think
    // it would be easier to do this as a process 

    pthread_t game_thread;

    thread_struct_t thread_args; 

    thread_args.duration = durations; 
    thread_args.index = letters; 
    thread_args.song_total_duration = num_durations; 

    pthread_create(&game_thread, NULL, run_game, &thread_args); 

    //Sleep to check for correct times
    usleep(55000); 

    // using arrays to create output 
    playMusic(frequencies, durations, num_durations); 
    
    pthread_join(game_thread, NULL); 

}

// Menu Options, quit, play again, see top scores, listen to some music


int main() { 

    // Initializing music functions globals
    music_func_init(); 

    //call play_game function if needed
    play_game(); 

    // Cleaning up the malloc'd memory we made in our data-structure
    cleanup_mem(); 

    return 0; 
}

