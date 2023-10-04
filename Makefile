# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files and output executable
SRC_DIR = src
OBJ_DIR = object
DB_DIR = data
EXE = SimpleDB

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Default target (the target that gets built when you run just 'make')
all: clean $(DB_DIR) $(EXE)

# Link the object files to create the executable
$(EXE): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ_FILES)

# Compile individual source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(EXE)

# Create the object directory if it doesn't exist
$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Create the database directory if it doesn't exist
$(DB_DIR):
	mkdir -p $(DB_DIR)

# Phony targets (targets that are not actual files)
.PHONY: all clean

