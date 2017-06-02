default: main grep cons window spawn filter tee cat

run: main
		./main

main: main.o struct.o
			gcc -o main main.o struct.o

main.o: main.c struct.h
			gcc -c -Wall main.c

struct.o: struct.c
			gcc -c -Wall struct.c

grep: grep.o
			gcc -o grep grep.o

grep.o: grep.c
			gcc -c -Wall grep.c

cons: cons.o
			gcc -o cons cons.o

cons.o: cons.c
			gcc -c -Wall cons.c

window: window.o
			gcc -o window window.o

window.o: window.c
			gcc -c -Wall window.c

spawn: spawn.o
			gcc -o spawn spawn.o

spawn.o: spawn.c
			gcc -c -Wall spawn.c

filter: filter.o
			gcc -o filter filter.o

filter.o: filter.c
			gcc -c -Wall filter.c

tee: tee.o
			gcc -o tee tee.o

tee.o: tee.c
			gcc -c -Wall tee.c

cat: cat.o
			gcc -o cat cat.o

cat.o: cat.c
			gcc -c -Wall cat.c

clean:
			-rm -f main
			-rm -f main.o
			-rm -f struct.o
			-rm -f cons.o grep.o window.o filter.o spawn.o tee.o cat.o
			-rm -f cons grep window filter spawn tee cat
