#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define SCALE_SIZE 8

// The range of acceptable values in the struct
int possible_values[8][8] = {
    {0, 1, 2, 3, 4, 5, 6, 7}, 
    {1, 4, 5, 6, 1, 4, 5, 6,},
    {0, 1, 2, 3, 4, 5, 6, 7}, 
    {1, 2, 5, 6, 1, 2, 5, 6,}, 
    {1, 2, 4, 6, 1, 2, 4, 6,}, 
    {1, 2, 4, 5, 1, 2, 4, 5}, 
    {1, 1, 1, 1, 1, 1, 1, 1}

}; 

/*
* Returns a winner of size SCALE SIZE, which would be 8, based off the location of the scale
*/
int return_winner(int location){
    int selected = rand() % SCALE_SIZE;
    return possible_values[location][selected]; 
}

/*
1: ALL 8
2: 1, 4, 5, 6
3: ALL 8
4: 1, 2, 5, 6
5: 1, 2, 4, 6
6: 1, 2, 4, 5
7: 1
*/