CC = g++

CFLAGS = -Wall -std=c++20 -O2 \
	-I$(CURDIR)/submodules/SDL3/include \
	-I$(CURDIR)/submodules/vulkan/include \
	-I$(CURDIR)/submodules/glm \
	-I$(CURDIR)/submodules/imgui \
	-I$(CURDIR)/submodules/imgui/backends

OUT = ./bin/pangaea

SRC = ./src/main.cpp 

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

.PHONY: all clean

clean:
	rm -f $(OUT)
