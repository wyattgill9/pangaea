CC = g++

CFLAGS = -Wall -std=c++20 -O2 \
	-I$(CURDIR)/submodules/SDL3/include \
	-I$(CURDIR)/submodules/vulkan/include \
	-I$(CURDIR)/submodules/glm \
	-I$(CURDIR)/submodules/imgui \
	-I$(CURDIR)/submodules/imgui/backends

ifeq ($(shell uname), Darwin)
    LDFLAGS += -L/opt/homebrew/Cellar/sdl3/3.2.10/lib \
               -L/opt/homebrew/lib \
               -Xlinker -rpath -Xlinker /opt/homebrew/lib \
               -lSDL3 -lvulkan -ldl -lpthread
else ifeq ($(shell uname), Linux)
    LDFLAGS += -L/usr/lib \
               -L/usr/lib/x86_64-linux-gnu \
               -lSDL3 -lvulkan -ldl -lpthread
endif

SRC = ./src/main.cpp \
	submodules/imgui/imgui.cpp \
	submodules/imgui/imgui_demo.cpp \
	submodules/imgui/imgui_draw.cpp \
	submodules/imgui/imgui_tables.cpp \
	submodules/imgui/imgui_widgets.cpp \
	submodules/imgui/backends/imgui_impl_sdl3.cpp \
	submodules/imgui/backends/imgui_impl_vulkan.cpp

OUT = ./bin/pangaea

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

.PHONY: all clean

clean:
	rm -f $(OUT)
