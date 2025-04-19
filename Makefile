CC = gcc
CFLAGS = -I/opt/homebrew/Cellar/sdl3/3.2.10/include -D_REENTRANT
LDFLAGS = -L/opt/homebrew/Cellar/sdl3/3.2.10/lib -lSDL3
SRC = src/main.c
OUT = bin/game

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)

.PHONY: all clean

