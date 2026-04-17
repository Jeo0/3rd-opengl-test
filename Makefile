EXE_BASE = build/example_glfw_opengl3

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = build/objects

# ---------------------------------------------------------------------
# automatically searches all subdirectories for matching files.
# ---------------------------------------------------------------------
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

# Automatically find ALL .cpp and .c files anywhere inside SRC_DIR
SOURCES_CPP := $(call rwildcard,$(SRC_DIR),*.cpp)
SOURCES_C   := $(call rwildcard,$(SRC_DIR),*.c)

# ---------------------------------------------------------------------
# 2. OBJECT PATH MIRRORING
# Convert src/folder/file.cpp to build/objects/src/folder/file.o
# ---------------------------------------------------------------------
OBJS_CPP := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SOURCES_CPP))
OBJS_C   := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES_C))
OBJS     := $(OBJS_CPP) $(OBJS_C)

CXX = g++
CC = gcc

INCLUDES = -I$(INC_DIR) \
		   -I$(INC_DIR)/gl

CXXFLAGS = -std=c++14 $(INCLUDES) -g -Wall -Wformat
CFLAGS = $(INCLUDES) -g -Wall -Wformat
LIBS =

# ---------------------------------------------------------------------
# OS DETECTION & PLATFORM SPECIFIC FLAGS
# ---------------------------------------------------------------------
ifeq ($(OS), Windows_NT)
    ECHO_MESSAGE = "MinGW (Windows)"
    LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32
    CXXFLAGS += `pkg-config --cflags glfw3`
    CFLAGS += `pkg-config --cflags glfw3`
    EXE = $(EXE_BASE).exe
    # Note: Ensure Windows users have a way to run mkdir -p, otherwise use powershell New-Item
    CLEAN_CMD = Remove-Item -Path '$(EXE)', '$(OBJ_DIR)' -Recurse -Force -ErrorAction SilentlyContinue
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Linux)
        ECHO_MESSAGE = "Linux"
        LIBS += -lGL `pkg-config --static --libs glfw3`
        CXXFLAGS += `pkg-config --cflags glfw3`
        CFLAGS += `pkg-config --cflags glfw3`
        EXE = $(EXE_BASE)
        CLEAN_CMD = rm -rf $(EXE) $(OBJ_DIR)
    endif
endif

# ---------------------------------------------------------------------
# BUILD RULES
# ---------------------------------------------------------------------
.PHONY: all run clean

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

run: all
	./$(EXE)

# Master rule for ALL C++ files
# The $(dir $@) creates the specific subfolder inside build/objects/ before compiling
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Master rule for ALL C files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Final Executable Linking
$(EXE): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	$(CLEAN_CMD)

