default: main grep.o cons.o window.o spawn.o filter.o

run: main
		./main

main: main.o struct.o
			gcc -o main main.o struct.o

main.o: main.c struct.h
			gcc -c -Wall main.c

struct.o: struct.c
			gcc -c -Wall struct.c

grep.o: grep.c
			gcc -c -Wall grep.c

cons.o: cons.c
			gcc -c -Wall cons.c

window.o: window.c
			gcc -c -Wall window.c

spawn.o: spawn.c
			gcc -c -Wall spawn.c

filter.o: filter.c
			gcc -c -Wall filter.c

clean:
			-rm -f main
			-rm -f main.o
			-rm -f struct.o
			-rm -f cons.o
			-rm -f grep.o
			-rm -f window.o
			-rm -f filter.o
			-rm -f spawn.o
