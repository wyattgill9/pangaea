CC = GCC
CFLAGS = -Wall -std=c11 -O2 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lglfw -lvulkan -ldl -lpthread
SRC = src/main.c
OUT = ./bin/pangaea

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

.PHONY: test clean

clean:
	rm -f $(OUT)

.PHONY: all clean
