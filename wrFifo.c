#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char const *argv[]) {
	int fich,f;
	char dados[80];
	f=open("1",O_RDONLY); /* abre o pipe com nome */
	for(;;)		{
		if (read(0,dados,sizeof(dados)) >0 )
      write(f,dados,sizeof(dados));
	}
	return 0;
}
