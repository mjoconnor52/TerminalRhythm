# TerminalRhythm
A terminal rhythm that we made for CSC 213

Project Created by Jim Deng, Sam Rudenberg and Mikey O'Connor

## Project Idea
This project is a Terminal Rhythm game that randomly generates music for you to play along with. It will also keep track of a points with a combo system and save it to the scores.txt file. The music will be determined by a randomly generated scale and played for approximately 20 seconds. If you want to increase the playing time, edit MAX_DURATIONS in the music-functions.c file. 

I bet you can't beat my high score of 1875. 

## Requirements
You will need to have the curses, SDL2 and clang libraries installed. 

## Running the Program
If you so desire, you can run game running the bash file, `runGame.bash` by using `bash runGame.bash` or you can do it manually with the following steps:
1. Go into the prototype directory `cd prototype`
2. Compile the program `make`
3. Run the program! `./prototype-exe`

