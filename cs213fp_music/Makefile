CC := clang
CFLAGS := -g -Wall -Wno-deprecated-declarations -Werror

all: music

clean:
	rm -f music

music: music.c 
	$(CC) $(CLAGS) -o music music.c -lSDL2 -lm 

