# Makefile

# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -pthread

# The build target executable
TARGET = st_pipeline

# Source files
SRCS = queue.c Active_Object.c main.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) -lm

clean:
	$(RM) $(TARGET)
