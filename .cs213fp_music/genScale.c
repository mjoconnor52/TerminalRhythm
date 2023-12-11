#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define WHOLE 2
#define HALF 1
#define NOTES_IN_SCALE 12
#define TUNING_FREQ 440

// 
typedef struct scales_info {
   double* scale; 
   double* rel; 
} scales_info_t; 

double generate_next_frequency(int note) {
   // the bitshift will make the 2^n value,
   return TUNING_FREQ *  pow(2, (note * (1 / (double) NOTES_IN_SCALE)));
}

// T
int distance_to_A(double frequency) {
   return round(NOTES_IN_SCALE * log2(frequency / (double) TUNING_FREQ));
}

double* generate_scale(double startingPitch, double *scaleType) {
   double * scale = malloc(sizeof(double) * NOTES_IN_SCALE);
   double start = 0; // where the equation will go after the pitch
   scale[0] = startingPitch; // This 0 is the index of the array

   start = distance_to_A(startingPitch);
   printf("\nStart: %d\n", start); 
   
   start = 0; 

   /// modifies freq to find freq in equation w/o saving
   if (startingPitch >= TUNING_FREQ) {
      while (round(generate_next_frequency(start)) != startingPitch) {
         start++;
      }
   }
   else {
      while (round(generate_next_frequency(start)) != startingPitch) {
         start--;
      }
   }

   // Increment each note in the scale
   for (int i = 1; i < NOTES_IN_SCALE; i++) {
      
      if (scaleType[i - 1] == HALF) {
         start++;
         scale[i] = generate_next_frequency(start);
      }
      else if (scaleType[i - 1] == WHOLE){ 
         start += 2;
         scale[i] = generate_next_frequency(start);
      }
   }

   return scale; 

}
