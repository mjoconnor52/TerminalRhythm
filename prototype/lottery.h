#pragma once 
#ifndef LOTTERY
#define LOTTERY

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "notes.h"

#define SCALE_SIZE 8
#define DUR_TYPES 5

int return_winner(int location);

double return_winner_dur(int location); 

#endif