EXE_BASE = build/example_glfw_opengl3

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./build/objects

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

CXXFLAGS = -std=c++17 $(INCLUDES)  -g -Wall -Wformat 
CFLAGS = $(INCLUDES)  -g -Wall -Wformat 
# EACH_OBJECT_FLAGS = -fsanitize=address
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
	CLEAN_CMD = rm -rf build
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S), Linux)
        ECHO_MESSAGE = "Linux"
        LIBS += -lGL `pkg-config --static --libs glfw3`
        CXXFLAGS += `pkg-config --cflags glfw3`
        CFLAGS += `pkg-config --cflags glfw3`
        EXE = $(EXE_BASE)
        CLEAN_CMD = rm -rf build
    endif
endif

# ---------------------------------------------------------------------
# BUILD RULES
# ---------------------------------------------------------------------
.PHONY: all run clean test unit zip

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



################################################################################
# SCRATCH / SPIKE PROGRAMS  ->  make test
#
# test/ holds one subfolder per thing you're exploring. Every .cpp inside
# a subfolder gets compiled and linked together into ONE executable named
# after that subfolder -- nothing from src/ is linked in automatically,
# and nothing here is checked for correctness. `make test` builds and
# runs every one of them, one after another. Add a new experiment by
# making a new folder under test/ with a main() in it; you do NOT need
# to touch this Makefile.
#
#   test/simdjsonModelParsing/   -> build/test/simdjsonModelParsing
#   test/assimpParsing/          -> build/test/assimpParsing
#   test/imguiImport/            -> build/test/imguiImport
#
# If one particular folder needs an extra library to link (e.g. assimp
# needs -lassimp), set it here BEFORE the $(foreach ... $(eval ...)) line
# below, named after the folder:
#   SCRATCH_LIBS_assimpParsing = -lassimp
################################################################################
SCRATCH_DIR     = ./test
SCRATCH_OBJ_DIR = ./build/test_objects
SCRATCH_BIN_DIR = ./build/test

# every direct subdirectory of test/ (test/foo/ -> foo) becomes its own target
SCRATCH_TARGETS := $(patsubst $(SCRATCH_DIR)/%/,%,$(wildcard $(SCRATCH_DIR)/*/))

# per-target library overrides -- empty unless you set one below, per the
# comment block above
# SCRATCH_LIBS_assimpParsing = -lassimp
# SCRATCH_LIBS_imguiImport   = ...

# per-target EXTRA existing project sources, for when a scratch program
# wants to reuse something real instead of just declarations -- e.g.
# simdjsonModelParsing needs simdjson.cpp itself compiled in, since
# simdjson.h is declaration-only. These MUST be set here, before the
# $(foreach ... $(eval ...)) block below generates each target's rule.
SCRATCH_EXTRA_SRCS_simdjsonModelParsing = $(SRC_DIR)/core/simdjson.cpp

# Generic compile rule: any .cpp under test/ (or pulled in via
# SCRATCH_EXTRA_SRCS_* above) mirrors into build/test_objects/, same idea
# as the main $(OBJ_DIR) rule above, using the same CXXFLAGS (so scratch
# code sees the same include paths -- simdjson, glm, GLFW, etc. -- as the
# real app).
$(SCRATCH_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# One instantiation of this per subfolder, via $(eval $(call ...)) below.
# $(1) is the subfolder/target name (e.g. "simdjsonModelParsing"). Doubled
# $$ throughout means "leave this for Make to expand later, not right now
# during the eval substitution" -- same as any other Make template. The
# EXTRA_SRCS lookup uses a single $ on purpose: it needs to resolve to a
# real path (or nothing) right now, while $(1) is being substituted.
define SCRATCH_template
SCRATCH_$(1)_SRCS := $$(call rwildcard,$(SCRATCH_DIR)/$(1),*.cpp) $(SCRATCH_EXTRA_SRCS_$(1))
SCRATCH_$(1)_OBJS := $$(patsubst %.cpp,$(SCRATCH_OBJ_DIR)/%.o,$$(SCRATCH_$(1)_SRCS))
SCRATCH_$(1)_BIN  := $(SCRATCH_BIN_DIR)/$(1)

$$(SCRATCH_$(1)_BIN): $$(SCRATCH_$(1)_OBJS)
	@mkdir -p $$(dir $$@)
	$$(CXX) -o $$@ $$^ $$(LIBS) $$(SCRATCH_LIBS_$(1))

.PHONY: test-$(1)
test-$(1): $$(SCRATCH_$(1)_BIN)
	@echo ""
	@echo "================== $(1) =================="
	@$$(SCRATCH_$(1)_BIN)
endef

$(foreach t,$(SCRATCH_TARGETS),$(eval $(call SCRATCH_template,$(t))))

test: $(foreach t,$(SCRATCH_TARGETS),test-$(t))



################################################################################
# UNIT TESTS  ->  make unit
#
# A separate, narrow build: only the source files a test actually needs,
# not "the whole app minus main.cpp". Right now that's just Model.cpp
# (the unit under test) and simdjson.cpp (its one real dependency).
# GL-touching classes (Mesh, Texture, Shader, ...) are NOT compiled in --
# see tests/support/gl_test_doubles.cpp for the couple of stand-in symbols
# Model.cpp still needs at link time. This keeps the test build fast and
# entirely GL-context-free. As Model.cpp grows to depend on more of the
# real app, pull in more real sources here (and trim the doubles file to
# match) rather than reaching for rwildcard.
################################################################################
UNIT_SRC_DIR = ./tests
UNIT_OBJ_DIR = ./build/unit_objects
UNIT_EXE     = ./build/run_tests

UNIT_SOURCES_CPP := \
	$(SRC_DIR)/gl/Model.cpp \
	$(SRC_DIR)/core/simdjson.cpp \
	$(call rwildcard,$(UNIT_SRC_DIR),*.cpp)

UNIT_OBJS := $(patsubst %.cpp, $(UNIT_OBJ_DIR)/%.o, $(UNIT_SOURCES_CPP))

# UNIT_TESTING flips Model's private section to public for this build only
# (see include/gl/Model.h) so tests can reach LoadBinaryData/TraverseNode.
UNIT_CXXFLAGS = -std=c++17 $(INCLUDES) -DUNIT_TESTING -g -Wall -Wformat

$(UNIT_OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(UNIT_CXXFLAGS) -c -o $@ $<

# No LIBS here on purpose: the unit build never links GLFW/GL, so it
# doesn't need them.
$(UNIT_EXE): $(UNIT_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) -o $@ $^

unit: $(UNIT_EXE)
	./$(UNIT_EXE)



# zipping the whole project
zip: 
	rm -i ./my_project.zip
	zip -r my_project.zip ./ -x "./.git/*" "./gitignore" "./logs.txt" "./externals/*" "./build/*" "./ghid/*" "./WHAT/*" "./neovide_backtraces.log" "./compile_commands.json" "./massif.out.118571" "./makefile2" "./leave_note" "./.cache/*" "./resource/Models/spear/*" "./resource/Models/stool/*" "./resource/Models/tricycle/*"

clean:
	$(CLEAN_CMD)
