To run:
make
./chash

or:
gcc -o chash chash.c hashdb.c rwlocks.c -lpthread
./chash

Windows does not support pthread and make natively, must be installed

AI prompt: 
could you make a Makefile for this:
Final Deliverables:
Filename	Purpose
chash.c	Your main program that reads the commands.txt and produces output to the console
hashdb.h	Your Concurrent Hash Table struct definitions
hashdb.c	Your Concurrent Hash Table implementation, including your Jenkins function and all linked list operations
rwlocks.h	Your Reader-Writer locks struct definitions
rwlocks.c	Your Reader-Writer locks implementation
common.h	If you used OSTEP's sample code, please include it.
common_threads.h	same.
Makefile	Your Makefile that builds this project into the final executable
README.txt	Use for anything I or my graders need to know and the AI use citation (see below)

response:
Sure, I can help you create a Makefile for your project. Here's a simple example:

```makefile
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
```

This Makefile will compile all your `.c` files into `.o` files and then link them to create an executable named `chash`. The `-Wall` and `-Wextra` flags are used for showing warnings, and the `-pthread` flag is used for multi-threading support.

Please note that this is a simple Makefile and might need to be adjusted based on your project's specific needs. For instance, if you have header files (`*.h`) in separate directories, or if you need to link against certain libraries, you would need to modify this Makefile accordingly. If you need further assistance, feel free to ask! 😊

