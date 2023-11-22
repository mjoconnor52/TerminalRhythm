// this file is for outputting the music 

// it is in an intermediate stage so a lot 
// is commented out to get it to compile 




#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "notes.c"
#include "userInput.c"

#define SAMPLE_RATE 44100
#define AMPLITUDE 32767

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


/* 
 * Create and outputs the audio 
 * 
 * \param song      This is a struct which stores key, frequencies array, num_frequencies 
 *                  in the frequencies array, durations array, and num_durations in the 
 *                  durations array. 
 *
*/

/* 

int playMusic(double song) {

    // Setting up a frequency array to store values in and pass to generateSineWave function 
    double frequencies[song.num_frequencies]; 
    // Copying the song's frequencies from the struct 
    memcpy(frequencies, song.frequencies, sizeof(song.frequencies)); 

    // Setting up a duration array to store values in and pass to generateSineWave function 
    double durations[song.num_durations]; 
    // copying the song's durations from the struct 
    memcpy(durations, song.durations, sizeof(song.durations)); 

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

    for (int i = 0; i < sizeof(frequencies) / sizeof(frequencies[0]); ++i) {
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

*/

int main() { 

    // store these from user menu 
    char* difficulty;
    char* emotion;

    // sample user input + copying 
    strcpy(difficulty, "Easy"); 
    strcpy(emotion, "Sad"); 

    // output song could be a struct with key, frequencies, num_frequencies, 
    // durations, and num_durations 
    // double song = inputs(difficulty, emotion); 

    // call playMusic with the song struct as a parameter to create audio 
    // playMusic(song); 

    return 0; 
}
