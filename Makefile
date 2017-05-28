default: main

run: main
		./main

main: main.o components.o struct.o
			gcc -o main main.o components.o struct.o

main.o: main.c components.h struct.h
			gcc -c -Wall main.c 

components.o: components.c
			gcc -c -Wall components.c

struct.o: struct.c
			gcc -c -Wall struct.c

clean:
			-rm -f main
			-rm -f main.o
			-rm -f components.o
			-rm -f struct.o
