#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "music-functions.h"

// This is where we will add user input 
int inputs(double frequencies[], int letters[], double durations[]) { 

   double selected_notes[MAX_NUM_FREQUENCIES]; 
   double selected_durations[MAX_NUM_DURATIONS]; 
   int selected_letters[MAX_NUM_FREQUENCIES]; 
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
   selected_letters[location] = location;  


   int genCount = 0; 
   double totalDuration = 0; 
   while (totalDuration < MAX_DURATION && genCount < MAX_NUM_FREQUENCIES) {
        location = return_winner(location); 
        selected_notes[genCount] = scale->scale[location];
        selected_letters[genCount] = location; 
        selected_durations[genCount] = return_winner_dur(location); 
        totalDuration += selected_durations[genCount];
        genCount++; 
   } 

    memcpy(frequencies, selected_notes, sizeof(double) * MAX_NUM_FREQUENCIES); 

    memcpy(letters, selected_letters, sizeof(int) * MAX_NUM_FREQUENCIES); 

    //int offset = 0; 

    memcpy(durations, selected_durations, sizeof(double) * MAX_NUM_DURATIONS); 

    return genCount; 

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
int playMusic(double frequencies[], double durations[], int count) {

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

    // In order to make the notes play somewhat unified, we will want to make the buffer bigger
    for (size_t i = 0; i < count; ++i) {
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
