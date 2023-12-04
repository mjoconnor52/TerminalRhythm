#pragma once
#ifndef MUSIC_FUNC
#define MUSIC_FUNC

#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "notes.h"
#include "lottery.h" 

#define SAMPLE_RATE 44100
#define AMPLITUDE 32767
#define MAX_NUM_FREQUENCIES 150 // Will need to change this based on song length / duration 
#define MAX_NUM_DURATIONS 150 // Will need to change this based on song length / duration 


/* Difficulty options: 
    Easy, Medium, Hard, Charlie 

    If we have time, we could have difficulty 
    change based on how well a player is doing 
*/
// An enum that was created for defining difficulty
enum Difficulty {
    Easy = 0, 
    Normal, 
    Hard, 
    Charlie, 
}; 

typedef struct notes{
    int frequency; 
    char keyboard_character; 
    int duration; 
} notes_t; 

void inputs(double frequencies[], char letters[], double durations[]); 

void generateSineWave(Uint8 *buffer, double frequency, double duration); 

int playMusic(double frequencies[], double durations[], size_t numFrequencies, size_t numDurations); 

void cleanup_mem(); 

#endif