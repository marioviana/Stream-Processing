#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define PIPE_BUF 100   /* Maximum length of event */

/* Auxiliar function to remove an element from an array */
void remove_element(char **array, int index, int array_length) {
  int i;
  for(i = index; i < array_length - 1; i++)
    array[i] = array[i + 1];
}

/* Auxiliar function to read line */
ssize_t readln(int fld, char *buf, size_t nbyte){
  int i=0;
  while ((read(fld,buf+i,1)>0) && i<nbyte && (buf[i]!='\n'))
    i++;
  buf[i]='\0';
  return i;
}

/* Window component: spawn <command> <args...> */
void spawn(int argc, char **argv){
  int x=0, a, i, r, arg[argc], trash[argc]; arg[0]=0;
  char buf[PIPE_BUF+1], bufcpy[PIPE_BUF+1], *del=":", *token, *statusS=NULL;
  for (i=1; i<argc; i++)
    (strstr(argv[i], "$") != NULL) ? (arg[i]=atoi(argv[i]+1)) : (arg[i]=0);
  while((r=(readln(0, buf, PIPE_BUF)))) {
    for (i=1; i<argc; i++) {
      a=1;
      if (arg[i]!=0) {
        strcpy(bufcpy, buf);
        token = strtok(bufcpy, del);
        if (arg[i]==1)
          argv[i] = token;
        else {
          while(a < arg[i] && ((token = strtok(NULL, del)) != NULL)) {
            a++;
          }
          if (arg[i] == a)
            strcpy(argv[i], token);
          else
            trash[i]=1;
        }
      }
    }
    for (i=1; i<argc; i++) {
      if (trash[i]==1) {
        remove_element(argv, i, argc);
        x++;
      }
    }
    argc-=x;
    int fexec = open("spawn.txt",O_CREAT| O_WRONLY|O_APPEND,0666);
    if (!fork()) {
      dup2(fexec, 1);
      close(fexec);
      execvp(argv[0], argv);
      _exit(1);
    }
    else {
      int status;
      wait(&status);
      if (WIFEXITED(status)) {
        if (WEXITSTATUS(status)==1)
          statusS="1";
        else
          statusS="0";
      }
      else
        statusS="1";
      if (r==PIPE_BUF || ((r+strlen(statusS)+1) > PIPE_BUF)) {
        write(1, buf, PIPE_BUF);
        write(1, "\n", 1);
      }
      else {
        strcat(buf, ":");
        strcat(buf, statusS);
        write(1, buf, (r+strlen(statusS)+1));
        write(1, "\n", 1);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  spawn(argc-1, argv+1);
  return 0;
}
