#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "lottery.h"

double possible_durs[DUR_TYPES] = {1, .25, .5, 2, 4}; 

// The range of acceptable values in the struct
int possible_values[8][8] = {
    {0, 1, 2, 3, 4, 5, 6, 7}, // 1 -> 1, 2, 3, 4, 5, 6, 7, 8
    {0, 3, 4, 5, 0, 3, 4, 5}, // 2 -> 1, 4, 5, 6 
    {0, 1, 2, 3, 4, 5, 6, 7}, // 3 -> 1, 2, 3, 4, 5, 6, 7, 8 
    {0, 1, 4, 5, 0, 1, 4, 5}, // 4 -> 1, 2, 5, 6 
    {0, 1, 3, 5, 0, 1, 3, 5}, // 5 -> 1, 2, 4, 6 
    {0, 1, 3, 4, 0, 1, 3, 4}, // 6 -> 1, 2, 4, 5 
    {0, 0, 0, 0, 0, 0, 0, 0}  // 7 -> 1 

}; 

/*
* Returns a winner of size SCALE SIZE, which would be 8, based off the location of the scale
*/
int return_winner(int location){
    int selected = rand() % SCALE_SIZE;
    return possible_values[location][selected]; 
}


double return_winner_dur(int location){
    return possible_durs[(rand() % DUR_TYPES)];    
}
