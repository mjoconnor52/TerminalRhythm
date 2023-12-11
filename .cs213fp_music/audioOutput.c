#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "music-functions.h"

int main() { 

    // initializing frequencies and durations arrays 
    double frequencies[MAX_NUM_FREQUENCIES]; 
    double durations[MAX_NUM_DURATIONS]; 
    char letters[MAX_NUM_DURATIONS]; 

    // generating music based on user input 
    // storing values in frequencies and durations arrays 
    inputs(frequencies, letters, durations); 

    for (int i = 0; i < MAX_DURATION; i++) { 
        printf("Frequency %d: %f Duration: %f\n", i+1, frequencies[i], durations[i]); 
    }

    // using arrays to create output 
    playMusic(frequencies, durations, MAX_NUM_FREQUENCIES, MAX_NUM_DURATIONS); 
    
    // Any other artifacts we make, we will want to clean up 
    // cleanup_mem(); 

    return 0; 
}

