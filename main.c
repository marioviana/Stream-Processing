#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "components.h"
#include "struct.h"

/* declaration of the global structure for nodes */
NodeList nodes[N_EVENTS];

void writeNode(int id, char buf[128], int r) {
  printf("Valor WRITE: %d\n", nodes[0]->nconW);
  int nconW = nodes[id-1]->nconW;
  if (nconW>0) {
    for(int i=0; i<nconW; i++){
      int length = snprintf( NULL, 0, "%d",  nodes[id-1]->conW[i]);
      char *strtmp = malloc (length + 1);
      snprintf (strtmp, length + 1, "%d", nodes[id-1]->conW[i]);
      int f = open(strtmp, O_WRONLY);
      strcat(buf, "\n");
      write(f, buf, r+1);
    }
  }
  else {
    strcat(buf, "\n");
    write(1, buf, r+1);
  }
}

void readlnNode(int id, char *buf, int r){
  int nconR = nodes[id-1]->nconR;
  if (nconR>0) {
    for(int i=0; i<nconR; i++){
      if (!fork()) {
        int length = snprintf( NULL, 0, "%d",  nodes[id-1]->conR[i]);
        char *strtmp = malloc (length + 1);
        snprintf (strtmp, length + 1, "%d", nodes[id-1]->conR[i]);
        int f = open(strtmp, O_RDONLY);
        readln(f, buf, r);
      }
    }
  }
  else {
    readln(0, buf, r);
  }
}

void connect(int argc, char **argv){
  int l, id = atoi(argv[0]);
  for(l=1; l<argc; l++) {
    nodes[id-1]->conW[nodes[id-1]->nconW] = atoi(argv[l]);
    nodes[id-1]->nconW++;
  }
  /*for(l=1; l<argc; l++) {
    id = atoi(argv[l]);
    nodes[id-1]->conR[nodes[id-1]->nconR] = atoi(argv[0]);
    nodes[id-1]->nconR++;
  }*/
}

void node(int argc, char **argv) {
  char *arg[argc], *cmd, buf[128], *idS = argv[0];
  int r, id, i, f;
  if (argc<2)
    printf("Error! Not enough arguments\n"); /* argc must be >= 2 */
  else {
    id = atoi(argv[0]);
    cmd = (char *) malloc (sizeof(char)*32);
    cmd = strdup(argv[1]);
    for(i=0;i<argc-2;i++)
      arg[i] = strdup(argv[i+2]);
    pid_t p = getpid();
    newNodeList(nodes, id,cmd,arg,i,p);
    write(1, "NODE\n", 5);
    if(!fork()){
      mkfifo(idS, 0666);
      f = open(idS, O_RDONLY);
      //int fd[2];
      while ((r=(readln(f, buf, 128)))) {
        char buf2[128];
        strcpy(buf2, buf);
        char *cmd = strtok(buf2, " ");
        if (!strcmp(cmd, "connect")) {
          char *token, *arg[20];
          int narg=0;
          while(((token = strtok(NULL, " ")) != NULL))
            arg[narg++] = token;
          connect(narg, arg);
          write(1, "NODE\n", 5);
        }
        /*cons(arg[1]);
        pipe(fd);
        if(!fork()){
          dup2(fd[0],0);
          dup2(fd[1],1);
          write(1, buf, r);
          readln(0, buf, r);
          close(fd[0]);
          close(fd[1]);
        }
        else {
          wait(0);
        }*/
        //readln(fd[0], buf, 128);
        //printf("%s\n", buf);
        else {
          write(1, "DENTRO DO NODE ", 15);
          write(1, idS, 1);
          write(1, "\n", 1);
          writeNode(id, buf, r);
        }
      }
      close(f);
      unlink(idS);
      _exit(0);
    }
  }
}

void inject (int argc, char **argv) {
  int r, f, file;
  char buf[PIPE_BUF];
  f = open(argv[0], O_WRONLY);
  if (!strcmp(argv[1], "cat")) {
    file = open(argv[2], O_RDONLY);
    while ((r=(readln(file, buf, PIPE_BUF)))) {
      strcat(buf, "\n");
      write(f, buf, r+1);
    }
    close(file);
  }
  else {
    while ((r=(readln(0, buf, PIPE_BUF)))) {
      //if(!fork()){
        strcat(buf, "\n");
        write(f, buf, r+1);
      //  _exit(0);
      //}
    }
    write(1, "Leaving inject...\n", 18);
  }
}

void rede(int argc, char **argv){
  int r, f = open(argv[0], O_RDONLY), narg, file;
  char buf[128], buf2[128], *cmd, *del=" ", *token, *arg[20];
  while ((r=(readln(f, buf, 128)))) {
    strcpy(buf2, buf);
    narg = 0;
    cmd = strtok(buf, del);
    while(((token = strtok(NULL, del)) != NULL))
      arg[narg++] = token;
    if (!(strcmp(cmd, "node")))
      node(narg, arg);
    else if (!(strcmp(cmd, "inject")))
      inject(narg, arg);
    else if (!(strcmp(cmd, "connect"))) {
      connect(narg, arg);
      file = open(arg[0], O_WRONLY);
      strcat(buf2, "\n");
      write(f, buf2, r+1);
    }
  }
  close(f);
}

int main(int argc, char **argv){
  int r, narg, f;
  char buf[128], buf2[128], *cmd, *del=" ", *token, *arg[20];
  initNodeList(nodes);
  while((r=(readln(0, buf, 128)))) {
    strcpy(buf2, buf);
    narg = 0;
    cmd = strtok(buf, del);
    while(((token = strtok(NULL, del)) != NULL))
      arg[narg++] = token;
    if (!(strcmp(cmd, "node")))
      node(narg, arg);
    else if (!(strcmp(cmd, "inject")))
      inject(narg, arg);
    else if (!(strcmp(cmd, "rede")))
      rede(narg, arg);
    else if (!(strcmp(cmd, "connect"))) {
      connect(narg, arg);
      f = open(arg[0], O_WRONLY);
      strcat(buf2, "\n");
      write(f, buf2, r+1);
    }
    printf("PID: %d\n", getpid());
  }
  return 0;
}
