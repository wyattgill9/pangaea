CC = G++
CFLAGS = -Wall -std=c++20 -O2 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lglfw -lvulkan -ldl -lpthread
SRC = src/main.cpp
OUT = ./bin/pangaea

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

.PHONY: test clean

clean:
	rm -f $(OUT)

.PHONY: all clean
