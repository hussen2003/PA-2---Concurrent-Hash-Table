all: chash

chash: chash.c hashdb.c rwlocks.c
	gcc -o chash chash.c hashdb.c rwlocks.c -lpthread
	./chash

clean:
	rm -f chash
