#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "lottery.h"

/** Defining durations in terms of a quarter note (1 beat)
 *  This allows for easy tempo changes 
 */ 
const double quarter = 1;
const double sixteenth = quarter / 4; 
const double eighth = quarter / 2; 
// const double half = quarter * 2; 
// const double whole = quarter * 4; 

/** 
 * Array with possible durations for notes 
 */ 
double possible_durs[DUR_TYPES] = {quarter, sixteenth, eighth}; 

/**
 * A 2-d array containing possible next pitches. 
 * There are 8 arrays of size 8. Each of the arrays 
 * corresponds to a scale degree 1 through 8. Each 
 * array has values within them to indicate which scale 
 * degrees are options for the next note. 
 *
 * Ex. possible_values[0][0-7] are the possible notes to 
 * move to when the current location is scale degree 1. 
 * 
 * Note: Add 1 to each value in the 2-d array to get it's 
 * corresponding scale degree 
 */
int possible_values[SCALE_SIZE][SCALE_SIZE] = {
                              // #  Can go to   Next Scale Deg: 
    {0, 1, 2, 3, 4, 5, 6, 7}, // 1  ->          1, 2, 3, 4, 5, 6, 7, 8
    {0, 3, 4, 5, 0, 3, 4, 5}, // 2  ->          1, 4, 5, 6 
    {0, 1, 2, 3, 4, 5, 6, 7}, // 3  ->          1, 2, 3, 4, 5, 6, 7, 8 
    {0, 1, 4, 5, 0, 1, 4, 5}, // 4  ->          1, 2, 5, 6 
    {0, 1, 3, 5, 0, 1, 3, 5}, // 5  ->          1, 2, 4, 6 
    {0, 1, 3, 4, 0, 1, 3, 4}, // 6  ->          1, 2, 4, 5 
    {0, 0, 0, 0, 0, 0, 0, 0}  // 7  ->          1 
}; 

/**
 * Randomly selects a number 0-7. 
 * Returns a winner based off the location of the scale. 
 * 
 * Ex. 5 is randomly generated, and the current location is 2. 
 * The return value will be the fifth index of the 2nd array in 
 * possible_values 2d array. 
 * 
 * \param location This is the current scale degree 
 * 
 */
int return_winner(int location){
    int selected = rand() % SCALE_SIZE;
    return possible_values[location][selected]; 
}

/**
 * Randomly selects a number 0-4. 
 * These are defined durations for each individual note. 
 */
double return_winner_dur(){
    return possible_durs[(rand() % DUR_TYPES)];    
}

