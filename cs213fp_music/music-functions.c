#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "music-functions.h"

#define SAMPLE_RATE 44100
#define AMPLITUDE 32767
#define MAX_NUM_FREQUENCIES 150 // Will need to change this based on song length / duration 
#define MAX_NUM_DURATIONS 150 // Will need to change this based on song length / duration 
#define MAX_DURATION 5

// The associated keys, chosen completly randomly
char keyboard_characters[NOTES_IN_SCALE] = {
    'A', 
    'S', 
    'D', 
    'F', 
    'H', 
    'J', 
    'K', 
    'L' 
}; 

// This is where we will add user input 
void inputs(double frequencies[], char letters[], double durations[]) { 

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

   double selected_notes[MAX_NUM_FREQUENCIES]; 
   double selected_durations[MAX_NUM_DURATIONS]; 
   char selected_letters[MAX_NUM_FREQUENCIES]; 
   //Starting at the tonic

   int location = 0; 

    // This will probably be some sort of selected enum.
    enum Scale_Mood mood = All; 

    // HASHMAP OPTION
    // Start creating our hashmap
    populate_hashmap(); 
    
    scales_info_t * scale = get_random_hashkey(mood);

   // Initalize the duration, tonic and letter
   selected_notes[location] = scale->scale[location];
   selected_durations[location] = return_winner_dur(location); 
   selected_letters[location] = keyboard_characters[location]; 

   printf("Scale Deg 1: %lf\n", scale->scale[0]); 

   int genCount = 0; 
   double totalDuration = 0; 
   while (totalDuration < MAX_DURATION && genCount < MAX_NUM_FREQUENCIES) {
        location = return_winner(location); 
        selected_notes[genCount] = scale->scale[location];
        selected_letters[genCount] = keyboard_characters[location]; 
        selected_durations[genCount] = return_winner_dur(location); 
        totalDuration += selected_durations[genCount];
        genCount++; 
   } 

   printf("genCount: %d totalDuration: %lf\n", genCount, totalDuration); 

   /* 
   Now, using the scale degrees, we can find the corresponding pitches from 
   the keySelection, and populate the frequencies array with these pitches
   */


    // for now, we can use the key C_Major, with 12 pitches in increasing order 
    memcpy(frequencies, selected_notes, sizeof(double) * MAX_NUM_FREQUENCIES); 

    //frequencies = realloc(frequencies, sizeof(double) * genCount); 

    // This tempDurations will need to be filled with some algorithm which also chooses notes 
    // within the scale 

    memcpy(letters, selected_letters, sizeof(char) * MAX_NUM_FREQUENCIES); 
    // Whole times
    //int offset = 0; 

    //letters = realloc(letters, sizeof(char) * genCount); 

    memcpy(durations, selected_durations, sizeof(double) * MAX_NUM_DURATIONS); 

    //durations = realloc(durations, sizeof(double) * genCount); 

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

    int check = 0; 

    while(frequencies[check] != 0 && durations[check] != 0) { 
        check++; 
    }

    printf("DURATION ARRAY SIZE: %d\n", check-1); 

    // In order to make the notes play somewhat unified, we will want to make the buffer bigger
    for (size_t i = 0; i < check; ++i) {
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

void cleanup_mem(){
    destroy_hashmap(); 
}


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