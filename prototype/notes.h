#pragma once

#ifndef NOTES
#define NOTES

#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_SCALES_IN_MOOD 100
#define NOTES_IN_OCTAVE 12
#define NOTES_IN_SCALE 8
#define STEPS_TO_MINOR 3
#define NUM_SCALES 12
#define TUNING_FREQ 440
#define WHOLE 2
#define HALF 1
#define DUR_TYPES 5

enum Scale_Mood{
   All,
   Happy, 
   Sad, 
   Dark, 
   Triumphant,
   Reflective
}; 

// A scale struct which will contain a scale, its relative major or minor, and whatever else we think might be fun. 
typedef struct scales_info{
   double* scale; 
   struct scales_info* rel; 
} scales_info_t; 

// A node for the hashmap, which will contain a key (a mood),
// an index to access the values array, the values (the keys)
// and then finally the next node
typedef struct hashmap_node
{
   enum Scale_Mood mood; 
   int index;
   scales_info_t *scales[MAX_SCALES_IN_MOOD];
   struct hashmap_node *next;
} hashmap_node_t;

// A header wrapper file
typedef struct hashmap_header
{
   hashmap_node_t *head;
} hashmap_header_t;

void hashmap_init(); 

void add_hashnode(enum Scale_Mood mood, scales_info_t * scale); 

scales_info_t *get_random_hashkey(enum Scale_Mood mood); 

void destory_scales_info(scales_info_t * info); 

void destroy_hashmap(); 

hashmap_header_t * get_list(); 

double * make_chord(double * scale, int chord_deg, int num_notes); 

double generate_frequency(int distance_to_A); 

int distance_to_A(double frequency); 

double* generate_scale(double startingPitch, int scaleType[]); 

void generate_rel_scales(int frequency, enum Scale_Mood maj_mood, enum Scale_Mood min_mood); 

void populate_hashmap(); 

#endif