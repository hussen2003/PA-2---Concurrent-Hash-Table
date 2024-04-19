# Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -pthread

# Source Files
SOURCES = chash.c hashdb.c rwlocks.c

# Object Files
OBJECTS = $(SOURCES:.c=.o)

# Executable Name
EXECUTABLE = chash

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

