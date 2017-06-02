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

/* Tee component */
void tee(int argc, char **argv) {
  int r, f[argc], i;
  for (i=0; i<argc; i++){
    f[i] = open(argv[i+1], O_CREAT | O_WRONLY);
  }
  char buf[PIPE_BUF];
  while((r=(readln(0, buf, PIPE_BUF)))) {
    strcat(buf, "\n");
    for (i=0; i<argc; i++){
      write(f[i], buf, r+1);
    }
    write(1, buf, r+1);
  }
  for (i=0; i<argc; i++){
    close(f[i]);
  }
}

int main(int argc, char *argv[]) {
  tee(argc, argv);
  return 0;
}
