CC := clang
CFLAGS := -g -Wall -Wno-deprecated-declarations -Werror

all: mus2

clean:
	rm -f mus2
	rm -f music

mus2: mus2.c music
	$(CC) $(CFLAGS) -o mus2 mus2.c -lSDL2 -lm -lSDL2_mixer

music: music.c 
	$(CC) $(CLAGS) -o music music.c -lSDL2 -lm 

