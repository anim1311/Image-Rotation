# Make file to compile the program

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Linker flags
LDFLAGS = -lm

# Name of the executable
EXEC = imageRotater

# List of object files
OBJ = main.o

# List of source files
SRC = main.c

# Default target
all: $(EXEC)

# Linking
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compiling
%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -o $@ -c $<

# Cleaning
clean:
	rm *.o

# Cleaning everything
mrproper: clean
	rm $(EXEC)

# End of Makefile

