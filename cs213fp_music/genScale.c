// This file is for defining notes, scales, triads
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define WHOLE 2
#define HALF 1
#define NOTES_IN_SCALE 12
#define TUNING_FREQ 440

// double generate_next_frequency(int note){
//    // the bitshift will make the 2^n value,
//    return TUNING_FREQ * pow(2, (note * (1 / (double) TUNING_FREQ) * (1 / (double)NOTES_IN_SCALE))); 
// }

double generate_next_frequency(int note){
   // the bitshift will make the 2^n value,
   return TUNING_FREQ * pow(2, (note  * (1 / (double)NOTES_IN_SCALE))); 
}

double * generate_scale(double startingPitch, double * scaleType) {

   double * scale = malloc(sizeof(double)*NOTES_IN_SCALE); 

   scale[0] = startingPitch; 
   int start = 0; 

   if(startingPitch >= TUNING_FREQ){
   while(round(generate_next_frequency(start)) != startingPitch){
    start++; 
   }
   } else {
       while(round(generate_next_frequency(start)) != startingPitch){
    start--; 
   }
   }


   for (int i = 1; i < NOTES_IN_SCALE; i++) {
        printf("Current Loop: %d\n", i); 
        //printf("scaleType[i-1]: %f\n", scaleType[i-1]); 

      if(scaleType[i-1] == HALF) { 
        printf("Half: %d\n", i);  
        start++; 
        scale[i] = generate_next_frequency(start);  
      } else if (scaleType[i-1] == WHOLE) { 
        start += 2;  
        scale[i] = generate_next_frequency(start); 
      }

   }

   return scale; 

} 

int main() { 

   int A5 = 294;

   double MajorScaleSteps[NOTES_IN_SCALE] = {WHOLE, WHOLE, HALF, WHOLE, WHOLE, WHOLE, HALF, WHOLE, WHOLE, HALF, WHOLE}; 
   double MinorScaleSteps[NOTES_IN_SCALE] = {WHOLE, HALF, WHOLE, WHOLE, HALF, WHOLE, WHOLE, WHOLE, HALF, WHOLE, WHOLE}; 

   double * storedScale; 

   storedScale = generate_scale(A5, MajorScaleSteps); 

   for (int i = 0; i < NOTES_IN_SCALE; i++){
      printf("%f\n", storedScale[i]); 
   }

   printf("NEXT FREQ: %f\n", generate_next_frequency(0)); 

}