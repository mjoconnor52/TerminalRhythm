CC := clang
CFLAGS := -g -Wall -Wno-deprecated-declarations -Werror

all: audioOutput

clean:
	rm -f audioOutput

audioOutput: audioOutput.c 
	$(CC) $(CLAGS) -o audioOutput audioOutput.c -lSDL2 -lm