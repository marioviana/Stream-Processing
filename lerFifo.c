#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char const *argv[]) {
	int fich,f;
	char dados[80];
	f=open("1",O_RDONLY); /* abre o pipe com nome */
	for(;;)		{
		if (read(f,dados,sizeof(dados)) >0 )
		printf("%s\n",dados);
	}
	return 0;
}
