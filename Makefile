CC      = gcc
CFL  = -g
RM      = rm -f

default: all

all: chash

chash: chash.c hashdb.c rwlocks.c
	$(CC) $(CFL) -o chash chash.c hashdb.c rwlocks.c -lpthread

clean:
	$(RM) chash
