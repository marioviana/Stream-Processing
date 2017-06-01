#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define PIPE_BUF 100

// int fildes -> 0 - stdin, 1 - stdout, 2 -

ssize_t readln(int fld, char *buf, size_t nbyte){
  int i=0;
  while ((read(fld,buf+i,1)>0) && i<nbyte && (buf[i]!='\n'))
    i++;
  buf[i]='\0';
  return i;
}

/* Const component: const <value> */
void cons(char *value, char *file1, char*file2){
  int r; char buf[PIPE_BUF+1];
  int f1 = open(file1, O_RDONLY);
  int f2 = open(file2, O_RDONLY);
  while((r=(readln(f1, buf, PIPE_BUF)))) {
    if (r==PIPE_BUF || ((r+strlen(value)+1) > PIPE_BUF)) {
      write(f2, buf, PIPE_BUF);
      write(f2, "\n", 1);
    }
    else {
      strcat(buf, ":");
      strcat(buf, value);
      write(f2, buf, (r+strlen(value)+1));
      write(f2, "\n", 1);
    }
  }
}

int main (int argc, char *argv[]){
  int r; char buf[PIPE_BUF];
  if(!fork()){
    execlp("./consTeste", "./consTeste", "10", "consTeste1", "constTeste2" , NULL);
    _exit(0);
  }
  else {
    if(!fork()){
      int f = open("consTeste1", O_WRONLY);
      while(1) {
        write(f, "OLÁ", 3);
      }
    }
    else {
      int f = open("constTeste2", O_RDONLY);
      while((r=(readln(f, buf, PIPE_BUF))))
        printf("- %s -\n", buf);
      }
  }
  return 0;
}
