# Variables
CC = gcc
CFLAGS = -lglut -lGL -lGLU -lm
OUT_DIR = out

# Set the executable path based on the input file
EXEC = $(OUT_DIR)/$(basename $(file))

# Default: compile and then execute
all: compile
	@./$(EXEC)

# Compilation logic
compile:
	@mkdir -p $(OUT_DIR)
	@$(CC) $(file) -o $(EXEC) $(CFLAGS)