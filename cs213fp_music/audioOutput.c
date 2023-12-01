#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "music-functions.h"

int main() { 

    // initializing frequencies and durations arrays 
    double frequencies[MAX_NUM_FREQUENCIES]; 
    double durations[MAX_NUM_DURATIONS]; 

    // generating music based on user input 
    // storing values in frequencies and durations arrays 
    inputs(frequencies, durations); 

    // using arrays to create output 
    playMusic(frequencies, durations, MAX_NUM_FREQUENCIES, MAX_NUM_DURATIONS); 
    
    // Any other artifacts we make, we will want to clean up
    //destroy_hashmap(); 

    return 0; 
}

