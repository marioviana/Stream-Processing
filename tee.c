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

/* Filter component: filter <column> <operation> <integer> */
void filter(char *value1, char *operation, char *value2) {
  int r;
  char buf[PIPE_BUF];
  while((r=(readln(0, buf, PIPE_BUF)))) {
  }
}

int main(int argc, char *argv[]) {
  tee(argv[1]);
  return 0;
}
