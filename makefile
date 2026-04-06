CC = gcc
CFLAGS = -lglut -lGL -lGLU -lm
OUT_DIR = out
DELAY = 1

EXEC = $(OUT_DIR)/$(basename $(file))

all: compile
	@echo "Compiled successfully."
	@sleep $(DELAY)
	@./$(EXEC)

compile:
	@mkdir -p $(OUT_DIR)
	@$(CC) $(file) -o $(EXEC) $(CFLAGS)