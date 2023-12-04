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
int MajorScaleSteps[NOTES_IN_SCALE] = {WHOLE, WHOLE, HALF, WHOLE, WHOLE, WHOLE, HALF}; 
int MinorScaleSteps[NOTES_IN_SCALE] = {WHOLE, HALF, WHOLE, WHOLE, HALF, WHOLE, WHOLE};

hashmap_header_t *mood_notes;

// Initializing the hashmap and random selection
void hashmap_init()
{
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
void add_hashnode(enum Scale_Mood mood, scales_info_t * scale)
{

   int sixth = 5; 

   // If mood_notes is null, we skip this entirely

   // creating a temp for the mood_notes to iterate through the list
   hashmap_node_t *current = mood_notes->head;

   // iterate through the list
   while (current != NULL)
   {
      // if we are able to find the item, we will add it to the values of our hashmap and we are done
      if (current->mood == mood)
      {
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
scales_info_t *get_random_hashkey(enum Scale_Mood mood)
{
   hashmap_node_t *current = mood_notes->head;

   // iterate through the list
   while (current != NULL)
   {
      // if we find the key we want, we will then randomly select a key from that list and then send it back
      if (current->mood == mood)
      {
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
double* generate_scale(double startingPitch, int scaleType[])
{
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
void generate_rel_scales(int frequency, enum Scale_Mood maj_mood, enum Scale_Mood min_mood){

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
   int major_freqs[NUM_SCALES] = {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 493}; 

   hashmap_init(); 

   for(int i =0; i < NUM_SCALES; i++){
      generate_rel_scales(major_freqs[i], All, All); 
   }


}




// /* octave 4 */
// const int C4 = 262;
// const int Df4 = 277;
// const int Cs4 = 277;
// const int D4 = 294;
// const int Ef4 = 311;
// const int Ds4 = 311;
// const int E4 = 330;
// const int F4 = 349;
// const int Gf4 = 370;
// const int Fs4 = 370;
// const int G4 = 392;
// const int Gs4 = 415;
// const int Af4 = 415;
// const int A4 = 440;
// const int Bf4 = 466;
// const int As4 = 466;
// const int B4 = 493;

// /* octave 5 */
// const int C5 = 523;
// const int Df5 = 554;
// const int Cs5 = 554;
// const int D5 = 587;
// const int Ef5 = 622;
// const int Ds5 = 622;
// const int E5 = 659;
// const int F5 = 698;
// const int Gf5 = 739;
// const int Fs5 = 739;
// const int G5 = 783;
// const int Af5 = 830;
// const int Gs5 = 830;
// const int A5 = 880;
// const int Bf5 = 932;
// const int As5 = 932;
// const int B5 = 988;

// /* octave 6 */
// const int C6 = 1047;
// const int Df6 = 1109;
// const int Cs6 = 1109;
// const int D6 = 1175;
// const int Ef6 = 1245;
// const int Ds6 = 1245;
// const int E6 = 1319;
// const int F6 = 1397;
// const int Gf6 = 1480;
// const int Fs6 = 1480;
// const int G6 = 1568;
// const int Af6 = 1661;
// const int Gs6 = 1661;
// const int A6 = 1760;
// const int Bf6 = 1865;
// const int As6 = 1865;
// const int B6 = 1976;


// /* Scales
//    Uses octaves 4-6
//    8 pitches per scale
//    Includes every semitone major/minor scale
// */
// double C_Major[NOTES_IN_SCALE] = {C4, D4, E4, F4, G4, A4, B4, C5, D5, E5, F5, G5};
// double A_Minor[NOTES_IN_SCALE] = {A4, B4, C5, D5, E5, F5, G5, A5, B5, C6, D6, E6};
// double C_Minor[NOTES_IN_SCALE] = {C4, D4, Ef4, F4, G4, Af4, Bf4, C5, D5, Ef5, F5, G5};
// double C_Sharp_Major[NOTES_IN_SCALE] = {Cs4, Ds4, F4, Fs4, Gs4, As4, C5, Cs5, Ds5, F5, Fs5, Gs5};
// double C_Sharp_Minor[NOTES_IN_SCALE] = {Cs4, Ds4, E4, Fs4, Gs4, A4, B5, Cs5, Ds5, E5, Fs5, Gs5};
// double D_Major[NOTES_IN_SCALE] = {D4, E4, Fs4, G4, A4, B4, Cs5, D5, E5, Fs5, G5, A5};
// double D_Minor[NOTES_IN_SCALE] = {D4, E4, F4, G4, A4, Bf4, C5, D5, E5, F5, G5, A5};
// double E_Flat_Major[NOTES_IN_SCALE] = {Ef4, F4, G4, Af4, Bf4, C5, D5, Ef5, F5, G5, Af5, Bf5};
// double E_Flat_Minor[NOTES_IN_SCALE] = {Ef4, F4, Gf4, Af4, Bf4, B4, Df5, Ef5, F5, Gf5, Af5, Bf5};
// double E_Major[NOTES_IN_SCALE] = {E4, Fs4, Gs4, A4, B4, Cs5, Ds5, E5, Fs5, Gs5, A5, B5};
// double E_Minor[NOTES_IN_SCALE] = {E4, Fs4, G4, A4, B4, C5, D5, E5, Fs5, G5, A5, B5};
// double F_Major[NOTES_IN_SCALE] = {F4, G4, A4, Bf4, C5, D5, E5, F5, G5, A5, Bf5, C6};
// double F_Minor[NOTES_IN_SCALE] = {F4, G4, Af4, Bf4, C5, Df5, Ef5, F5, G5, Af5, Bf5, C6};
// double G_Flat_Major[NOTES_IN_SCALE] = {Gf4, Af4, Bf4, B5, Df5, Ef5, F5, Gf5, Af5, Bf5, Df6, Ef6};
// double G_Flat_Minor[NOTES_IN_SCALE] = {Gf4, Af4, A4, B4, Df5, D5, E5, Gf5, Af5, A5, B5, Df6};
// double G_Major[NOTES_IN_SCALE] = {G4, A4, B4, C5, D5, E5, Fs5, G5, A5, B5, C6, D6};
// double G_Minor[NOTES_IN_SCALE] = {G4, A4, Bf4, C5, D5, Ef5, F5, G5, A5, Bf5, C6, D6};
// double A_Flat_Major[NOTES_IN_SCALE] = {Af4, Bf4, C5, Df5, Ef5, F5, G5, Af5, Bf5, C6, Df6, Ef6};
// double A_Flat_Minor[NOTES_IN_SCALE] = {Af4, Bf4, B4, Df5, Ef5, E5, Gf5, Af5, Bf5, B5, Df6, Ef6};
// double A_Major[NOTES_IN_SCALE] = {A4, B4, Cs5, D5, E5, Fs5, Gs5, A5, B5, Cs6, D6, E6};
// double B_Flat_Major[NOTES_IN_SCALE] = {Bf4, C5, D5, Ef5, F5, G5, A5, Bf5, C6, D6, Ef6, F6};
// double B_Flat_Minor[NOTES_IN_SCALE] = {Bf4, C5, Df5, Ef5, F5, Gf5, Af5, Bf5, C6, Df6, Ef6, F6};
// double B_Major[NOTES_IN_SCALE] = {B4, Cs5, Ds5, E5, Fs5, Gs5, As5, B5, Cs6, Ds6, E6, Fs6};
// double B_Minor[NOTES_IN_SCALE] = {B4, Cs5, D5, E5, Fs5, G5, A5, B5, Cs6, D6, E6, Fs6};

// double allKeys[NUM_SCALES][NOTES_IN_SCALE];


// double * happyKeys[11] = {C_Major, D_Major, E_Major, G_Major, A_Major,
//                         C_Sharp_Major, E_Flat_Major, G_Flat_Major, A_Flat_Major,
//                         B_Flat_Major, B_Major};

// double* reflectiveKeys[9] = {C_Minor, E_Minor, A_Minor, C_Sharp_Minor, E_Flat_Minor,
//                             G_Flat_Minor, A_Flat_Minor, B_Flat_Minor, B_Minor};

// double* sadKeys[10] = {D_Minor, F_Minor, G_Minor, A_Minor, B_Minor, C_Sharp_Minor,
//                       E_Flat_Minor, G_Flat_Minor, A_Flat_Minor, B_Flat_Minor};

// double* darkKeys[10] = {F_Minor, G_Minor, A_Minor, B_Minor, G_Flat_Major, A_Flat_Major,
//                        B_Flat_Major, G_Flat_Minor, A_Flat_Minor, B_Flat_Minor};

// double* triumphantKeys[3] = {D_Major, B_Major, E_Flat_Major};


// // I think the hashamp will be preferable if we want to continue with the struct, 
// // but the array implementation straight up won't be terrible either
// double* get_random_scale(enum Scale_Mood mood){
//    double * scale = malloc(sizeof(double) * NOTES_IN_SCALE); 
//    switch (mood)
//    {
//    case Happy:
//       memcpy(scale, happyKeys[rand() % 11], NOTES_IN_SCALE); 
//       break;
//    case Sad: 
//       memcpy(scale, sadKeys[rand() % 10], NOTES_IN_SCALE); 
//       break;
//    case Dark: 
//       memcpy(scale, darkKeys[rand() % 10], NOTES_IN_SCALE); 
//       break;   
//    case Triumphant: 
//       memcpy(scale, triumphantKeys[rand() % 3], NOTES_IN_SCALE); 
//       break;
//    case Reflective: 
//       memcpy(scale, triumphantKeys[rand() % 9], NOTES_IN_SCALE); 
//       break;    
//    default:
//       memcpy(scale, happyKeys[rand() % 11], NOTES_IN_SCALE); 
//       break;
//    }
//    return scale; 
// }


// /* Chords */
// We have created a function that generates theses 
// double i[TRIAD] = {0, 2, 4};
// double ii[TRIAD] = {1, 3, 5};
// double iii[TRIAD] = {2, 4, 6};
// double iv[TRIAD] = {3, 5, 7};
// double v[TRIAD] = {4, 6, 8};
// double vi[TRIAD] = {5, 7, 9};
// double vii[TRIAD] = {6, 8, 10};
// double viii[TRIAD] = {7, 9, 11};

// /* octave 4 */
// const int C4 = 262;
// const int Df4 = 277;
// const int Cs4 = 277;
// const int D4 = 294;
// const int Ef4 = 311;
// const int Ds4 = 311;
// const int E4 = 330;
// const int F4 = 349;
// const int Gf4 = 370;
// const int Fs4 = 370;
// const int G4 = 392;
// const int Gs4 = 415;
// const int Af4 = 415;
// const int A4 = 440;
// const int Bf4 = 466;
// const int As4 = 466;
// const int B4 = 493;


// double C_Major[NOTES_IN_SCALE] = {C4, D4, E4, F4, G4, A4, B4, C5, D5, E5, F5, G5};
// double C_Minor[NOTES_IN_SCALE] = {C4, D4, Ef4, F4, G4, Af4, Bf4, C5, D5, Ef5, F5, G5};
// double C_Sharp_Major[NOTES_IN_SCALE] = {Cs4, Ds4, F4, Fs4, Gs4, As4, C5, Cs5, Ds5, F5, Fs5, Gs5};
// double C_Sharp_Minor[NOTES_IN_SCALE] = {Cs4, Ds4, E4, Fs4, Gs4, A4, B5, Cs5, Ds5, E5, Fs5, Gs5};
// double D_Major[NOTES_IN_SCALE] = {D4, E4, Fs4, G4, A4, B4, Cs5, D5, E5, Fs5, G5, A5};
// double D_Minor[NOTES_IN_SCALE] = {D4, E4, F4, G4, A4, Bf4, C5, D5, E5, F5, G5, A5};
// double E_Flat_Major[NOTES_IN_SCALE] = {Ef4, F4, G4, Af4, Bf4, C5, D5, Ef5, F5, G5, Af5, Bf5};
// double E_Flat_Minor[NOTES_IN_SCALE] = {Ef4, F4, Gf4, Af4, Bf4, B4, Df5, Ef5, F5, Gf5, Af5, Bf5};
// double E_Major[NOTES_IN_SCALE] = {E4, Fs4, Gs4, A4, B4, Cs5, Ds5, E5, Fs5, Gs5, A5, B5};
// double E_Minor[NOTES_IN_SCALE] = {E4, Fs4, G4, A4, B4, C5, D5, E5, Fs5, G5, A5, B5};
// double F_Major[NOTES_IN_SCALE] = {F4, G4, A4, Bf4, C5, D5, E5, F5, G5, A5, Bf5, C6};
// double F_Minor[NOTES_IN_SCALE] = {F4, G4, Af4, Bf4, C5, Df5, Ef5, F5, G5, Af5, Bf5, C6};
// double G_Flat_Major[NOTES_IN_SCALE] = {Gf4, Af4, Bf4, B5, Df5, Ef5, F5, Gf5, Af5, Bf5, Df6, Ef6};
// double G_Flat_Minor[NOTES_IN_SCALE] = {Gf4, Af4, A4, B4, Df5, D5, E5, Gf5, Af5, A5, B5, Df6};
// double G_Major[NOTES_IN_SCALE] = {G4, A4, B4, C5, D5, E5, Fs5, G5, A5, B5, C6, D6};
// double G_Minor[NOTES_IN_SCALE] = {G4, A4, Bf4, C5, D5, Ef5, F5, G5, A5, Bf5, C6, D6};
// double A_Flat_Major[NOTES_IN_SCALE] = {Af4, Bf4, C5, Df5, Ef5, F5, G5, Af5, Bf5, C6, Df6, Ef6};
// double A_Flat_Minor[NOTES_IN_SCALE] = {Af4, Bf4, B4, Df5, Ef5, E5, Gf5, Af5, Bf5, B5, Df6, Ef6};
// double A_Major[NOTES_IN_SCALE] = {A4, B4, Cs5, D5, E5, Fs5, Gs5, A5, B5, Cs6, D6, E6};
// double A_Minor[NOTES_IN_SCALE] = {A4, B4, C5, D5, E5, F5, G5, A5, B5, C6, D6, E6};
// double B_Flat_Major[NOTES_IN_SCALE] = {Bf4, C5, D5, Ef5, F5, G5, A5, Bf5, C6, D6, Ef6, F6};
// double B_Flat_Minor[NOTES_IN_SCALE] = {Bf4, C5, Df5, Ef5, F5, Gf5, Af5, Bf5, C6, Df6, Ef6, F6};
// double B_Major[NOTES_IN_SCALE] = {B4, Cs5, Ds5, E5, Fs5, Gs5, As5, B5, Cs6, Ds6, E6, Fs6};
// double B_Minor[NOTES_IN_SCALE] = {B4, Cs5, D5, E5, Fs5, G5, A5, B5, Cs6, D6, E6, Fs6};

// double * happyKeys[11] = {C_Major, D_Major, E_Major, G_Major, A_Major,
//                         C_Sharp_Major, E_Flat_Major, G_Flat_Major, A_Flat_Major,
//                         B_Flat_Major, B_Major};

// double* reflectiveKeys[9] = {C_Minor, E_Minor, A_Minor, C_Sharp_Minor, E_Flat_Minor,
//                             G_Flat_Minor, A_Flat_Minor, B_Flat_Minor, B_Minor};

// double* sadKeys[10] = {D_Minor, F_Minor, G_Minor, A_Minor, B_Minor, C_Sharp_Minor,
//                       E_Flat_Minor, G_Flat_Minor, A_Flat_Minor, B_Flat_Minor};

// double* darkKeys[10] = {F_Minor, G_Minor, A_Minor, B_Minor, G_Flat_Major, A_Flat_Major,
//                        B_Flat_Major, G_Flat_Minor, A_Flat_Minor, B_Flat_Minor};

// double* triumphantKeys[3] = {D_Major, B_Major, E_Flat_Major};


// Was attempting to use char*'s for a for loop such as:

// for (int i = 0; i < 24; i++) {
//    memcpy(allKeys[i], allKeyNames[i], sizeof(allKeyNames[i]));
// }

// char* allKeyNames = {C_Major, C_Minor, C_Sharp_Major, C_Sharp_Minor, D_Major, D_Minor,
//                      E_Flat_Major, E_Flat_Minor, E_Major, E_Minor, F_Major, F_Minor,
//                      G_Flat_Major, G_Flat_Minor, G_Major, G_Minor, A_Flat_Major, A_Flat_Minor,
//                      A_Major, A_Minor, B_Flat_Major, B_Flat_Minor, B_Major, B_Minor};

// also tried this version but it gave errors

// memcpy(allKeys[0], C_Major, sizeof(C_Major));
// memcpy(allKeys[1], C_Minor, sizeof(C_Minor));
// memcpy(allKeys[2], C_Sharp_Major, sizeof(C_Sharp_Major));
// memcpy(allKeys[3], C_Sharp_Minor, sizeof(C_Sharp_Minor));
// memcpy(allKeys[4], D_Major, sizeof(D_Major));
// memcpy(allKeys[5], D_Minor, sizeof(D_Minor));
// memcpy(allKeys[6], E_Flat_Major, sizeof(E_Flat_Major));
// memcpy(allKeys[7], E_Flat_Minor, sizeof(E_Flat_Minor));
// memcpy(allKeys[8], E_Major, sizeof(E_Major));
// memcpy(allKeys[9], E_Minor, sizeof(E_Minor));
// memcpy(allKeys[10], F_Major, sizeof(F_Major));
// memcpy(allKeys[11], F_Minor, sizeof(F_Minor));
// memcpy(allKeys[12], G_Flat_Major, sizeof(G_Flat_Major));
// memcpy(allKeys[13], G_Flat_Minor, sizeof(G_Flat_Minor));
// memcpy(allKeys[14], G_Major, sizeof(G_Major));
// memcpy(allKeys[15], G_Minor, sizeof(G_Minor));
// memcpy(allKeys[16], A_Flat_Major, sizeof(A_Flat_Major));
// memcpy(allKeys[17], A_Flat_Minor, sizeof(A_Flat_Minor));
// memcpy(allKeys[18], A_Major, sizeof(A_Major));
// memcpy(allKeys[19], A_Minor, sizeof(A_Minor));
// memcpy(allKeys[20], B_Flat_Major, sizeof(B_Flat_Major));
// memcpy(allKeys[21], B_Flat_Minor, sizeof(B_Flat_Minor));
// memcpy(allKeys[22], B_Major, sizeof(B_Major));
// memcpy(allKeys[23], B_Minor, sizeof(B_Minor));


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