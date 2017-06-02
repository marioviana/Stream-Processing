default: controlador grep const window spawn filter tee cat

run: controlador
		./controlador

controlador: controlador.o struct.o
			gcc -o controlador controlador.o struct.o

controlador.o: controlador.c struct.h
			gcc -c -Wall controlador.c

struct.o: struct.c
			gcc -c -Wall struct.c

grep: grep.o
			gcc -o grep grep.o

grep.o: grep.c
			gcc -c -Wall grep.c

const: const.o
			gcc -o const const.o

const.o: const.c
			gcc -c -Wall const.c

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
			-rm -f controlador
			-rm -f controlador.o
			-rm -f struct.o
			-rm -f const.o grep.o window.o filter.o spawn.o tee.o cat.o
			-rm -f const grep window filter spawn tee cat
