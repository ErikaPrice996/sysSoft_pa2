# Compiler / flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread -std=c99
LDFLAGS = -pthread

# Source files
SRCS = chash.c insert.c delete.c search.c print.c update.c
OBJS = $(SRCS:.c=.o)
TARGET = chash

# Default target
all: $(TARGET)

# Link executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET) hash.log

# Run the program
run: $(TARGET)
	./$(TARGET)

# Rebuild from scratch
rebuild: clean all

.PHONY: all clean run rebuild