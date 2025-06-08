MAINFILE := src/*.cpp
MAINFILE2 := src/*.c
INCLUDE1 = include
INCLUDE2 = resource/Shaders
LIB = lib
CC = g++
LINK = -lglfw -lGL -ldl -lpthread -lwayland-client -lwayland-cursor -lwayland-egl
DEBUGFLAGS := -g -Wall -Wextra
OUT = run.out

# for cross platform devleopment
ifeq ($(OS),Windows_NT)
    LINK = -lglfw3 -lopengl32 -lkernel32 -luser32 -lgdi32 -lwinmm
    OUT = run.exe
else
		# for x11
    # LINK = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

		# for wayland
		LINK = -lglfw -lGL -ldl -lpthread -lwayland-client -lwayland-cursor -lwayland-egl
    OUT = run.out
endif

all:
	$(CC) $(MAINFILE) $(MAINFILE2) \
		-I$(INCLUDE1) \
		-I$(INCLUDE2) \
		-L$(LIB) \
		$(DEBUGFLAGS) \
		$(LINK) \
		-o $(OUT)
	./$(OUT)

