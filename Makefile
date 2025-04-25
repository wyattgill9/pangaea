CC = G++

CFLAGS = -Wall -std=c++20 -O2 \
	-I$(CURDIR)/submodules/SDL3/include \
	-I$(CURDIR)/submodules/bgfx/include \
	-I$(CURDIR)/submodules/glm

OUT = ./bin/pangaea

SRC = ./src/main.cpp 

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

.PHONY: all clean

clean:
	rm -f $(OUT)

