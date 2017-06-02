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
  int i, r, v1, v2, v3, c1, c2;
  char buf[PIPE_BUF+1], *aux[PIPE_BUF+1], bufcpy[PIPE_BUF+1];
  v1 = atoi(value1);
  v2 = atoi(value2);
  v3 = v1>v2?v1:v2; // v3 = max v1 v2

  while((r=(readln(0, buf, PIPE_BUF)))) {
    int x, count;
    char *buf2 = strdup(buf);
    for (x=0, count=0; buf2[x]; x++)
      count += (buf2[x] == ':');
    strcpy(bufcpy, buf);
    aux[0] = strtok(bufcpy, ":");
    i=1;
    while(i<=(v3-1)){
      aux[i] = strtok(NULL, ":");
      i++;
    }
    if(count<(atoi(value2)-1)){
      write(1, "\n", 1);
    }
    else if(!(strcmp(operation, "="))){
      c1 = atoi(aux[v1-1]);
      c2 = atoi(aux[v2-1]);
      if(c1==c2) {
        write(1, buf, r);
        write(1, "\n", 1);
      }
    }
    else if(!(strcmp(operation, "<"))){
      c1 = atoi(aux[v1-1]);
      c2 = atoi(aux[v2-1]);
      if(c1<c2) {
        write(1, buf, r);
        write(1, "\n", 1);
      }
    }
    else if(!(strcmp(operation, "<="))){
      c1 = atoi(aux[v1-1]);
      c2 = atoi(aux[v2-1]);
      if(c1<=c2) {
        write(1, buf, r);
        write(1, "\n", 1);
      }
    }
    else if(!(strcmp(operation, ">"))){
      c1 = atoi(aux[v1-1]);
      c2 = atoi(aux[v2-1]);
      if(c1>c2) {
        write(1, buf, r);
        write(1, "\n", 1);
      }
    }
    else if(!(strcmp(operation, ">="))){
      c1 = atoi(aux[v1-1]);
      c2 = atoi(aux[v2-1]);
      if(c1>=c2) {
        write(1, buf, r);
        write(1, "\n", 1);
      }
    }
    else if(!(strcmp(operation, "!="))){
      c1 = atoi(aux[v1-1]);
      c2 = atoi(aux[v2-1]);
      if(c1!=c2) {
        write(1, buf, r);
        write(1, "\n", 1);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  filter(argv[1], argv[2], argv[3]);
  return 0;
}
