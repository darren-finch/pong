SRC_DIR = src
BUILD_DIR = build/debug
CC = gcc
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_NAME = play
#-I /opt/homebrew/opt/sdl2/include -I /opt/homebrew/opt/sdl2_image/include -I /opt/homebrew/opt/sdl2_ttf/include
INCLUDE_PATHS = -I/opt/homebrew/Cellar/sdl2/2.28.3/include/ -I/opt/homebrew/Cellar/sdl2/2.28.3/include/SDL2/ -I/opt/homebrew/Cellar/sdl2_ttf/2.20.2/include/
LIBRARY_PATHS = -L/opt/homebrew/Cellar/sdl2/2.28.3/lib/ -L/opt/homebrew/Cellar/sdl2_ttf/2.20.2/lib/
COMPILER_FLAGS = -std=c11 -Wall -O0 -g
#-lsdl2_image
LINKER_FLAGS = -lsdl2 -lsdl2_ttf

all:
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) -o $(BUILD_DIR)/$(OBJ_NAME)