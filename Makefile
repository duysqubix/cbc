# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -Wa,-ahlms=output.s # Enable all warnings and debugging info
LDFLAGS =          # You can add linker flags if needed (e.g., -lm for math library)

# Executable name
TARGET = gameboy

# Source files and object files
SRCS = main.c gameboy.c opcodes.c
OBJS = main.o gameboy.o opcodes.o

# Default target to build the project
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile each .c file into .o object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


# Rule to clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# PHONY target to avoid conflict with files named 'clean' or 'all'
.PHONY: clean all
