all: main

main: config.o mmem.o memb.o malloc.o malloc_compact.o realloc.o list.o main.o
	gcc config.o mmem.o memb.o malloc.o malloc_compact.o realloc.o list.o main.o -o main

config.o: config.c config.h contiki-conf.h
	gcc config.c -c -o config.o

mmem.o: lib/mmem.c lib/mmem.h contiki-conf.h
	gcc lib/mmem.c -c -o mmem.o

memb.o: lib/memb.c lib/memb.h
	gcc lib/memb.c -c -o memb.o
	
malloc.o: lib/malloc.c lib/malloc.h
	gcc lib/malloc.c -c -o malloc.o

malloc_compact.o: lib/malloc_compact.c lib/malloc.h
	gcc lib/malloc_compact.c -c -o malloc_compact.o

realloc.o: lib/realloc.c lib/malloc.h
	gcc lib/realloc.c -c -o realloc.o

list.o: lib/list.c lib/list.h
	gcc lib/list.c -c -o list.o

main.o: main.c
	gcc main.c -c -o main.o

clean:
	rm -f *.o main
