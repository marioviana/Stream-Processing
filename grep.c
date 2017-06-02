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

/* Grep component: grep <column> <value> */
void grep(char *col, char *search){
  int a, r;
  char buf[PIPE_BUF+1], bufcpy[PIPE_BUF+1], *del=":", *token;
  while((r=(readln(0, buf, PIPE_BUF)))) {
    a=1;
    strcpy(bufcpy, buf);
    if  (strstr(buf, ":") != NULL) {
      token = strtok(bufcpy, del);
      while (a < atoi(col) && ((token = strtok(NULL, del)) != NULL))
        a++;
      if (!(strcmp(token, search))){
        write(1, buf, r);
        write(1, "\n", 1);
      }
      else {
        write(1, "\n", 1);
      }
    }
    else {
      write(1, "\n", 1);
    }
  }
}

int main(int argc, char *argv[]) {
  grep(argv[1], argv[2]);
  return 0;
}
