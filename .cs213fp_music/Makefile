CC := clang
CFLAGS := -g -Wall -Wno-deprecated-declarations -Werror 
LFLAGS:= LDFLAGS

all: audioOutput

clean:
	rm -f *.o


audioOutput: audioOutput.c  music-functions.o notes.o lottery.o
	$(CC) $(CFLAGS) -o audioOutput.exe audioOutput.c music-functions.o lottery.o notes.o -lSDL2 -lm

notes.o: notes.c notes.h
	$(CC) $(CFLAGS)  -c notes.c

lottery.o: lottery.c lottery.h 
	$(CC) $(CFLAGS)  -c $< 

music-function.o: music-functions.c music-functions.h 
	$(CC) $(CFLAGS) -c $< 



	