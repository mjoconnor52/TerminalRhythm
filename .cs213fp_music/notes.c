// This file is for defining notes, scales, triads
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "notes.h"

// Modify to make it encapsulated

const double quarter = 1;
const double sixteenth = quarter / 4; 
const double eighth = quarter / 2; 
const double half = quarter * 2; 
const double whole = quarter * 4; 

// Local constants for the creation function
double MajorScaleSteps[NOTES_IN_SCALE] = {WHOLE, WHOLE, HALF, WHOLE, WHOLE, WHOLE, HALF, WHOLE, WHOLE, HALF, WHOLE};
double MinorScaleSteps[NOTES_IN_SCALE] = {WHOLE, HALF, WHOLE, WHOLE, HALF, WHOLE, WHOLE, WHOLE, HALF, WHOLE, WHOLE};
double ChromaticScaleSteps[NOTES_IN_SCALE] = {HALF, HALF, HALF, HALF, HALF, HALF, HALF, HALF, HALF, HALF, HALF}; 

hashmap_header_t *mood_notes;

// Initializing the hashmap and random selection
void hashmap_init(){
   mood_notes = malloc(sizeof(hashmap_node_t));
   mood_notes->head = NULL;
   srand(time(NULL));
}

/**
 * A function that will add a node into the hashnode based on a mood, the size of the string,
 * and the scale we are looking to add. This will be very similar to adding into a traditional hashmap
 *
 * \param mood A string that represnts the mood of the key
 * \param scale The array of doubles we are adding into the key
 *
 */
void add_hashnode(enum Scale_Mood mood, scales_info_t * scale){

   int sixth = 5; 

   // If mood_notes is null, we skip this entirely

   // creating a temp for the mood_notes to iterate through the list
   hashmap_node_t *current = mood_notes->head;

   // iterate through the list
   while (current != NULL){
      // if we are able to find the item, we will add it to the values of our hashmap and we are done
      if (current->mood == mood){
         // To Do: Change to loop 
         // I want to avoid possible repeats in the code, I chose the sixth but idk if that's the best
         if(scale->scale[sixth] == current->scales[current->index]->scale[sixth ])
            return; 

         current->index++;
         current->scales[current->index] = scale;
         return;
      }

      // Otherwise keep iterating through
      current = current->next;
   }

   // We need to make a new node for our list, add the fields we want, and then update the list
   hashmap_node_t *new_node = malloc(sizeof(hashmap_node_t));
   new_node->mood = mood;
   new_node->index = 0;
   new_node->scales[new_node->index] = scale;
   new_node->next = mood_notes->head;
   mood_notes->head = new_node;
}

/** A function that will get a random key from the selected mood. Since we are only going to ever use one key
 * (for the time being), we will just randomly select a key. We might also want to modify this if we ever want to
 * do any keychanges or anything. This should maybe be fairly fast, so I hope if we want to do a key change, we can do
 * it fairly quickly.
 *
 * \param mood A string that represents the mood of the key
 *
 * \return an array of doubles
 */
scales_info_t *get_random_hashkey(enum Scale_Mood mood){
   hashmap_node_t *current = mood_notes->head;

   // iterate through the list
   while (current != NULL){
      // if we find the key we want, we will then randomly select a key from that list and then send it back
      if (current->mood == mood){
         int selected_key = current->index == 0 ? 0 : rand() % current->index;
         return current->scales[selected_key];
      }

      // Otherwise keep iterating through
      current = current->next;
   }

   // If we are unable to fine the scale, we will return NULL where the calling function will have to check
   printf("Could not find mood, please try again");
   return NULL;
}

/*
* This function will clean up itself in the other scales
*/
void destory_scales_info(scales_info_t * info){
   double* scale = info->scale; 
   scales_info_t* rscale = info->rel; 
   free(scale); 
   free(rscale); 
}

/**
 * Destroys the hashmap, use at end to avoid memory leaks
*/
void destroy_hashmap()
{
   // List traversal
   hashmap_node_t *current = mood_notes->head;

   while (current != NULL)
   {
      // Collecting the malloc'd info and freeing it
      hashmap_node_t *temp = current;
      //free all scales inside of temp_scales
      for(int i = 0; i< MAX_SCALES_IN_MOOD; i++){
         // creating the 
         scales_info_t * temp_scales =  temp->scales[i]; 
         if(temp->scales[i] != NULL){
            destory_scales_info(temp_scales); 
         } else{
            free(temp_scales);
            break; 
         }
      }

      // Otherwise keep iterating through
      current = current->next;
      free(temp);
   }
   free(mood_notes);
}

/**
 * Retrieves the global list variable (Probably for debug purposes, we probably want to encapulate it here)
*/
hashmap_header_t * get_list(){
   return mood_notes; 
}

/**
 * This will make a num_notes chord based on the scale and degree you give it. It will return a populated chord in
 * memory. You will want to free the chord when you are done with it
 * 
 * \param scale The scale needed to make the chord
 * \param chord_deg The deg of the chord 
 * \param num_notes The number of notes in the chord
 * 
 * \return Retuns a chord of `chord_deg`, from `scale` scale with `num_notes` notes
 * 
*/
double * make_chord(double * scale, int chord_deg, int num_notes){
   // the chord array, which will have the freq of the chords
   double * chord = malloc(sizeof(double) * 3); 

   //Going though each position in the scale to get the appropriate scale degree
   for(int i = 0; i < num_notes; i += 2){
      chord[i] = scale[i]; 
   }

   return chord; 
}

/** This function will generate the frequency of a given interval from A4
 *  NEEDS OPTOMIZATION
 * Inital Equation: 440 * x^(n/12)
 * 
 * \param distance_to_A The relative distance the note is to A4
 * \ref https://www.intmath.com/trigonometric-graphs/music.php 
 * \return The frequency of the next note on the piano
*/
double generate_frequency(int distance_to_A){
   // the bitshift will make the 2^n value,
   return TUNING_FREQ * pow(2, (distance_to_A  * (1 / (double)NOTES_IN_SCALE))); 
}

/**
 * A function that basically does the inverse of the generate next frequency, given a frequency, it will generate the 
 * interval from A4
 * \param frequency The frequency of the note
 * \return the distance the frequency is from A4 or 440 HZ based on the standardized music equation
*/
int distance_to_A(double frequency){
   return round(NOTES_IN_SCALE * log2(frequency / (double) TUNING_FREQ));
}

// I feel this will be the bottleneck in our generation
// We will probably want to talk to charlie to optomize it
/* This function will generate and return  a scale given a pitch and scale typed
 *
 * \param startingPitch The inital pitch of the note
 * \param scaleType The Type of the Scale (Major, Minor, etc.)
 * \return A scale of the starting pitch to the ending pitch (which will be the scale param)
 *  I did this because it will be easier when we make the scale struct 
 */
double* generate_scale(double startingPitch, double scaleType[]){
   double * scale = malloc(sizeof(double) * NOTES_IN_SCALE);
   scale[0] = startingPitch; // This 0 is the index of the array

   //Finds the initial distance from A4
   int start = distance_to_A(startingPitch); 

   // // modifies freq to find freq in equation w/o saving
   // if (startingPitch >= TUNING_FREQ){
   //    while (round(generate_frequency(start)) != startingPitch){
   //       start++;
   //    }
   // }
   // else {
   //    while (round(generate_frequency(start)) != startingPitch){
   //       start--;
   //    }
   // }

   // Increment each note in the scale
   for (int i = 1; i < NOTES_IN_SCALE; i++)
   {
      if (scaleType[i - 1] == HALF){
         start++;
         scale[i] = generate_frequency(start);
      }
      else if (scaleType[i - 1] == WHOLE){
         start += 2;
         scale[i] = generate_frequency(start);
      }
   }

   return scale;
}

/**
 * Assuming freq is the `frequency` for the first note in the major scale, we will then generate it's major, 
 * relative minor and whatever other scales we feel like and then add it to the hashtable with `mood` 
 * 
 * \param frequency The first note in the major scale of the frequency
 * \param mood An enum representation of the mood
 * 
 * \returns Side effect: Affects the hashmap
 * 
*/
void generate_rel_scales(double frequency, enum Scale_Mood maj_mood, enum Scale_Mood min_mood){

   scales_info_t *storedMajor = malloc(sizeof(scales_info_t));
   scales_info_t *storedMinor = malloc(sizeof(scales_info_t));

   // We will free scale in destroy hashmap (the hashmap is responsible for the memory)
   storedMajor->scale = generate_scale(frequency, MajorScaleSteps);

   // Really crappy way to fix this but its the only way I can think of doing the rel. minor
   storedMinor->rel = storedMajor; 

   //Math for finding stored Minor
   int minor_dist = (distance_to_A(frequency) - STEPS_TO_MINOR); 

   storedMinor->scale = generate_scale(generate_frequency(minor_dist), MajorScaleSteps); 
   storedMajor->rel = storedMinor; 

   add_hashnode(maj_mood, storedMajor); 
   add_hashnode(min_mood, storedMinor); 
}

/**
 * This function will create, and populate the hashmap
*/
void populate_hashmap(){
   double major_freqs[NUM_SCALES] = {261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3, 440, 466.16, 493.88}; 

   hashmap_init(); 

   for(int i =0; i < NUM_SCALES; i++){
      generate_rel_scales(major_freqs[i], All, All); 
   }


}

// /** This function will generate the next frequency/note once given an inital note, I tried to optomize the equation so it is faster
//  * 
//  * \param The frequency of the current note 
//  * \ref https://www.intmath.com/trigonometric-graphs/music.php 
//  * \return The frequency of the next note on the piano
// */
// double generate_next_frequency(int note){
//    // the bitshift will make the 2^n value,
//    return TUNING_FREQ * 2 << (note * (1 / NOTES_IN_SCALE)); 
// }