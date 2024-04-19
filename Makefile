all: chash

chash: chash.c hashdb.c rwlocks.c
	gcc -o chash chash.c hashdb.c rwlocks.c -lpthread

clean:
	rm -f chash
