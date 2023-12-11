CC := clang
CFLAGS := -g -Wall -Wno-deprecated-declarations -Werror -fsanitize=address
LFLAGS:= LDFLAGS

all: game-manager

clean:
	rm -f *.o
	rm -f audioOutput
	rm -f prototype


game-manager: game-manager.c  music-functions.o notes.o lottery.o ui.o
	$(CC) $(CFLAGS) -o prototype-exe game-manager.c music-functions.o lottery.o notes.o ui.o -lSDL2 -lm -lncurses -lpthread

notes.o: notes.c notes.h
	$(CC) $(CFLAGS)  -c notes.c

lottery.o: lottery.c lottery.h 
	$(CC) $(CFLAGS)  -c $< 

music-function.o: music-functions.c music-functions.h 
	$(CC) $(CFLAGS) -c $< 

ui.o: ui.c ui.h
	$(CC) $(CFLAGS) -c $<


	