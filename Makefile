SRC_DIR = src
BUILD_DIR = build/debug
CC = gcc
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_NAME = play
INCLUDE_PATHS = -I include/SDL2/include -I include/SDL2_image/include
LIBRARY_PATHS = -L lib/SDL2 -L lib/SDL2_image
COMPILER_FLAGS = -std=c11 -Wall -O0 -g
LINKER_FLAGS = -lsdl2 -lsdl2_image

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)