all: main

main: config.o module.o module_manager.o mmem.o memb.o malloc.o malloc_compact.o realloc.o list.o main.o
	gcc config.o module.o module_manager.o mmem.o memb.o malloc.o malloc_compact.o realloc.o list.o main.o -g -o main

config.o: config.c config.h contiki-conf.h
	gcc config.c -c -g -o config.o

module.o: module.c module.h
	gcc module.c -c -g -o module.o
	
module_manager.o: module_manager.c module_manager.h
	gcc module_manager.c -c -g -o module_manager.o

mmem.o: lib/mmem.c lib/mmem.h contiki-conf.h
	gcc lib/mmem.c -c -g -o mmem.o

memb.o: lib/memb.c lib/memb.h
	gcc lib/memb.c -c -g -o memb.o
	
malloc.o: lib/malloc.c lib/malloc.h
	gcc lib/malloc.c -c -g -o malloc.o

malloc_compact.o: lib/malloc_compact.c lib/malloc.h
	gcc lib/malloc_compact.c -c -g -o malloc_compact.o

realloc.o: lib/realloc.c lib/malloc.h
	gcc lib/realloc.c -c -g -o realloc.o

list.o: lib/list.c lib/list.h
	gcc lib/list.c -c -g -o list.o

main.o: main.c
	gcc main.c -c -g -o main.o

clean:
	rm -f *.o main
