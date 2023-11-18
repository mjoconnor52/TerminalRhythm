#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>

#define NUM_NOTES 7

// Function to play a note with a specific frequency and duration
void playNote(int frequency, int duration) {
    Mix_Chunk *note = Mix_QuickLoad_RAW((Uint8*) "\0", 1);

    int sampleRate = 44100;
    int amplitude = 32760;  // Adjust as needed

    int numSamples = sampleRate * duration;
    double increment = 2.0 * M_PI * frequency / sampleRate;

    Mix_SetDistance(255);  // Max volume

    for (int i = 0; i < numSamples; ++i) {
        Sint16 sample = (Sint16)(amplitude * sin(increment * i));
        Mix_QuickLoad_RAW((Uint8*)&sample, 2);
        Mix_PlayChannel(-1, note, 0);

        // Delay for the duration of each sample
        SDL_Delay(1000 * duration / numSamples);
    }

    Mix_FreeChunk(note);
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096) < 0) {
        fprintf(stderr, "SDL_mixer initialization failed: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    // Define a simple melody (frequency in Hz, duration in milliseconds)
    int frequencies[NUM_NOTES] = {440, 523, 659, 783, 987, 1175, 1319};
    int durations[NUM_NOTES] = {500, 500, 500, 500, 500, 500, 500};

    // Play each note in the melody
    for (int i = 0; i < NUM_NOTES; ++i) {
        playNote(frequencies[i], durations[i]);
    }

    // Wait for the melody to finish playing
    SDL_Delay(1000 * durations[NUM_NOTES - 1]);

    // Clean up SDL_mixer and SDL
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}
