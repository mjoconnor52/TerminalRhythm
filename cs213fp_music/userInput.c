
// This file is for user inputs







/* Difficulty options: 

    Easy 
    Medium 
    Hard 
    Charlie 

    If we have time, we could have difficulty 
    change based on how well a player is doing 

*/

/* Descriptor: an emotion 

    Choose a musical key based on emotion. 
    This could be done in many ways... 

    Starting simple, we could just choose a few basic 
    emotions and sort the keys under these. 

    A way to add complexity: Use a file of emotional 
    words. We could make our own few categories, such 
    as happy, sad, angry, confused, and group the music 
    keys into these. Then we could sort the emotional 
    words from the file by these categories. A user will 
    type in a word. If it doesn't match a word in the file 
    they would be asked to try again, otherwise if it has 
    a match in the emotional words file, it would check the 
    categorical emotion, find the keys in that group, and 
    then (if there are multiple) randomly select one key. 
    
*/

/* Take in user inputs
    Return a struct with the frequencies and durations */
void inputs(char* difficulty, char* descriptor) { 

    /*cmaybe enum or something else would make more sense 
    in order to equate 0 is Easy, 1 is Medium, etc. */ 

    int difficultyRating = -1; 

    if (strcmp(difficulty, "Easy") == 0) { 
        difficultyRating = 0; 
    } else if (strcmp(difficulty, "Medium") == 0) { 
        difficultyRating = 1; 
    } else if (strcmp(difficulty, "Hard") == 0) { 
        difficultyRating = 2; 
    } else if (strcmp(difficulty, "Charlie") == 0) { 
        difficultyRating = 3; 
    }

}

