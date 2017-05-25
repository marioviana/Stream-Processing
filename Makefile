default: main

main: main.o
			gcc main.o -o main

main.o: main.c
			gcc -c -Wall main.c -o main.o

clean:
			-rm -f main
			-rm -f main.o
