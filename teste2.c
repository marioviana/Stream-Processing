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


int main (int argc, char *argv[]){
  char buf[PIPE_BUF];
   char buf1[PIPE_BUF];
  int r = readln(0, buf, PIPE_BUF);
  strcat(buf, "\n");
  //mkfifo("component", 0666);
  //f = open("component", O_RDWR);
  //write(f, buf, r);
  int fd[2];
  int fd1[2];
  pipe(fd);
  pipe(fd1);
  int pid;
  if(pid = !fork()){
    dup2(fd[1],1);
    dup2(fd1[0],0);
    close(fd[1]);
    close(fd[0]);
    close(fd1[1]);
    close(fd1[0]);
    execlp("./cons", "./cons", "10", NULL);
    _exit(1);
  }
  else {
      write(fd1[1],buf,r+1);
      dup2(fd[0],0);
      close(fd[1]);
      close(fd[0]);
      close(fd1[1]);
      close(fd1[0]);
      int w;
      while(w=readln(0,buf1,r+4)){
          //write(1,buf1,w);
          //write(1,"\n",1);
      }

  }
}
