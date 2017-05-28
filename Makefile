default: main

main: main.o components.o struct.o
			gcc -o main main.o components.o struct.o

main.o: main.c components.h struct.h
			gcc -c -Wall main.c components.h struct.h

components.o: components.c components.h
			gcc -c -Wall components.c components.h

struct.o: struct.c struct.h
			gcc -c -Wall struct.c struct.h

clean:
			-rm -f main
			-rm -f main.o
			-rm -f components.o
			-rm -f struct.o
