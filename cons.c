#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define PIPE_BUF 100   /* Maximum length of event */

/* Auxiliar function to read line */
ssize_t readln(int fld, char *buf, size_t nbyte){
  int i=0;
  while ((read(fld,buf+i,1)>0) && i<nbyte && (buf[i]!='\n'))
    i++;
  buf[i]='\0';
  return i;
}


/* Const component: const <value> */
void cons(char *value){
  int r; char buf[PIPE_BUF+1];
  while((r=(readln(0, buf, PIPE_BUF)))) {
    if (r==PIPE_BUF || ((r+strlen(value)+1) > PIPE_BUF)) {
      write(1, buf, PIPE_BUF);
      write(1, "\n", 1);
    }
    else {
      strcat(buf, ":");
      strcat(buf, value);
      write(1, buf, (r+strlen(value)+1));
      write(1, "\n", 1);
    }
  }
}

int main(int argc, char *argv[]) {
  cons(argv[1]);
  return 0;
}
