#include <unistd.h> //system codes of unix
#include <stdio.h>
#include <fcntl.h>

#define PIPE_BUF 100

// int fildes -> 0 - stdin, 1 - stdout, 2 -

ssize_t readln(int fld, char *buf, size_t nbyte){
  int i=0;
  while ((read(fld,buf+i,1)>0) && i<nbyte && (buf[i]!='\n'))
    i++;
  buf[i]='\0';
  return i;
}

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

int main (int argc, char *argv[]){
  char buf[100];
  int r, fd[2];
  while((r=(readln(0, buf, 100)))) {
    pipe(fd);
    cons(10);
    if(!fork()) {
      close(fd[0]);
      dup2(fd[1],1);
      write(1, buf, r);
      close(fd[1]);
      _exit(0);
    }
    else {
      wait(0);
      close(fd[1]);
      dup2(fd[0],0);
      readln(0, buf, r+2);
      close(fd[0]);
      printf("DENTRO DO PIPE\n");
      printf("%s\n", buf);
    }
    //readln(fd[0], buf, 128);
    //printf("%s\n", buf);
    printf("DENTRO DO PIPE\n");
    printf("%s\n", buf);
  }
}
