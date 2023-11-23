#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "notes.c" 

#define SAMPLE_RATE 44100
#define AMPLITUDE 32767
#define MAX_NUM_FREQUENCIES 12 // Will need to change this based on song length / duration 
#define MAX_NUM_DURATIONS 12 // Will need to change this based on song length / duration 

// This is where we will add user input 
void inputs(double frequencies[], double durations[]) { 

    double keySelection[MAX_NUM_FREQUENCIES]; 

    /*
    if statements here to choose one of these sorted 2d-arrays 
      -> allKeys 
      -> happyKeys 
      -> reflectiveKeys 
      -> sadKeys 
      -> darkKeys 
      -> triumphantKeys
    */ 

    /* 
    After choosing a 2d-array, we can randomly (or methodically) 
    select one of its rows and memcpy that to keySelection 
    */ 

    /* 
    An example call choosing a specific key 
    memcpy(keySelection, allKeys[4], sizeof(allKeys[4])); 
    */ 

    /*
    Here is the place were we will generate a logical order 
    of scale degrees to follow each other. 

    For example, scale degree 1 and 8 are tonic, scale degree 5 is dominant, 
    2, 4, and 6 are subdominant. 
    
    Suppose we start on scale degree 1, the tonic. 
    From tonic, we move to subdominant 2, 4, or 6. 
    From subdominant, we can move back to tonic 1, or move to dominant 5. 
    From dominant 5, we can move to subdominant 4 or 6, or back to tonic 1. 
    These are the general rules, we can also throw in scale degrees 3 and 7 
    and make other random exeception rules. 
    */

    /* 
    Now, using the scale degrees, we can find the corresponding pitches from 
    the keySelection, and populate the frequencies array with these pitches
    */

    // memcpy(frequencies, keySelection, sizeof(keySelection)); 

    // for now, we can use the key C_Major, with 12 pitches in increasing order 
    memcpy(frequencies, C_Major, sizeof(C_Major)); 

    // This tempDurations will need to be filled with some algorithm which also chooses notes 
    // within the scale 
    double tempDurations[] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}; 

    memcpy(durations, tempDurations, sizeof(tempDurations)); 

} 

// This function is done 
void generateSineWave(Uint8 *buffer, double frequency, double duration) {
    Uint32 length = (Uint32)(SAMPLE_RATE * duration);
    double timeStep = 1.0 / SAMPLE_RATE;

    for (Uint32 i = 0; i < length; ++i) {
        double t = i * timeStep;
        double wave = sin(2.0 * M_PI * frequency * t);
        Sint16 sample = (Sint16)(wave * AMPLITUDE);

        buffer[i * 2] = (Uint8)(sample & 0xFF);
        buffer[i * 2 + 1] = (Uint8)((sample >> 8) & 0xFF);
    }
}

// This function is done 
int playMusic(double frequencies[], double durations[], size_t numFrequencies, size_t numDurations) {

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_AudioSpec audioSpec;
    audioSpec.freq = SAMPLE_RATE;
    audioSpec.format = AUDIO_S16SYS;
    audioSpec.channels = 1;  // 1 for mono, 2 for stereo
    audioSpec.samples = 4096;
    audioSpec.callback = NULL;

    SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(NULL, 0, &audioSpec, NULL, 0);
    if (audioDevice == 0) {
        fprintf(stderr, "SDL could not open audio device! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_PauseAudioDevice(audioDevice, 0);

    for (size_t i = 0; i < numFrequencies; ++i) {
        Uint32 length = (Uint32)(SAMPLE_RATE * durations[i]);
        Uint8 *buffer = (Uint8 *)malloc(length * 2);  // 2 bytes per sample for AUDIO_S16SYS

        generateSineWave(buffer, frequencies[i], durations[i]);
        SDL_QueueAudio(audioDevice, buffer, length * 2);

        free(buffer);

        // Wait for the sound to finish
        SDL_Delay((Uint32)(durations[i] * 1000));
    }

    SDL_CloseAudioDevice(audioDevice);
    SDL_Quit();

    return 0;
} 

/* Difficulty options: 
    Easy, Medium, Hard, Charlie 

    If we have time, we could have difficulty 
    change based on how well a player is doing 
*/

/* Descriptor: an emotion 
    -> Choose a musical key based on emotion. 
       This could be done in many ways... 
    -> Starting simple, we could just choose a few basic 
       emotions and sort the keys under these. 
    -> A way to add complexity: Use a file of emotional 
       words. We could make our own few categories, such 
       as happy, sad, angry, confused, and group the music 
       keys into these. Then we could sort the emotional 
       words from the file by these categories. A user will 
       type in a word. If it doesn't match a word in the file 
       they would be asked to try again, otherwise if it has 
       a match in the emotional words file, it would check the 
       categorical emotion, find the keys in that group, and 
       then (if there are multiple) randomly select one key. 
*/
// typedef struct song { 
//     int * difficulty; 
//     char* descriptor; 
//     double frequencies[]; 
//     int * num_frequencies; 
//     double durations[]; 
//     int * num_durations; 
// } song_t; 

/* maybe enum or something else would make more sense 
    in order to equate 0 is Easy, 1 is Medium, etc. */ 

    // int difficultyRating = -1; 

    // if (strcmp(difficulty, "Easy") == 0) { 
    //     song->difficulty = 0; 
    // } else if (strcmp(difficulty, "Medium") == 0) { 
    //     song->difficulty = 1; 
    // } else if (strcmp(difficulty, "Hard") == 0) { 
    //     song->difficulty = 2; 
    // } else if (strcmp(difficulty, "Charlie") == 0) { 
    //     song->difficulty = 3; 
    // }