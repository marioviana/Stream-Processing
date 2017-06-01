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
  int idN = existNode(nodes, id);
  int nconW = nodes[idN]->nconW;
  printf("Valor WRITE: %d\n", nodes[idN]->conW[0]);
  if (nconW>0) {
    for(int i=0; i<nconW; i++){
      int length = snprintf( NULL, 0, "%d",  nodes[idN]->conW[i]);
      char *strtmp = malloc (length + 1);
      snprintf (strtmp, length + 1, "%d", nodes[idN]->conW[i]);
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

/*void readlnNode(int id, char *buf, int r){
  int idN = existNode(nodes, id);
  int nconR = nodes[idN]->nconR;
  if (nconR>0) {
    for(int i=0; i<nconR; i++){
      if (!fork()) {
        int length = snprintf( NULL, 0, "%d",  nodes[idN]->conR[i]);
        char *strtmp = malloc (length + 1);
        snprintf (strtmp, length + 1, "%d", nodes[idN]->conR[i]);
        int f = open(strtmp, O_RDONLY);
        readln(f, buf, r);
      }
    }
  }
  else {
    readln(0, buf, r);
  }
}*/

void connect(int argc, char **argv){
  int l, j, id = atoi(argv[0]), idN = existNode(nodes, id), r=0;
  for(l=1; l<argc; l++) {
    for(j=0; j<nodes[idN]->nconW; j++){
      if(nodes[idN]->conW[j]==atoi(argv[l])) {
        r=1;
        break;
      }
    }
    if(!r) {
      nodes[idN]->conW[nodes[idN]->nconW] = atoi(argv[l]);
      nodes[idN]->nconW++;
    }
    r=0;
  }
}

void disconnect(char *id1, char *id2){
  int i, j, idN1 = existNode(nodes, atoi(id1));// idN2 = existNode(nodes, atoi(id2));
  int ncon1 = nodes[idN1]->nconW;// ncon2 = nodes[idN2]->nconR;
  for(i=0; i<ncon1; i++){
    if(nodes[idN1]->conW[i]==atoi(id2)) {
      for(j = i; j < ncon1 - 1; j++) {
        nodes[idN1]->conW[j] = nodes[idN1]->conW[j + 1];
      }
      nodes[idN1]->nconW--;
      break;
    }
  }
}

void removeStruct(char *id1, char *id2, char *id3){
  char *arg[2]; arg[0] = id1; arg[1] = id3;
  disconnect(id1, id2);
  connect(2, arg);
}

void remov(char *id){
  int idN = existNode(nodes, atoi(id)), nconW = nodes[idN]->nconW;
  int j, l, t;// *conW = intdup(nodes[idN]->conW, nconW);
  for(j=0; nodes[j] != NULL; j++){
    for(l=0; l<nodes[j]->nconW ; l++){
      if (nodes[j]->conW[l]==atoi(id)){ // j = 1
        int length = snprintf( NULL, 0, "%d",  nodes[j]->id);
        char *strtmp = malloc (length + 1);
        snprintf (strtmp, length + 1, "%d", nodes[j]->id);
        int f = open(strtmp, O_WRONLY);
        for (t=0; t<nconW; t++){
          char string[50];
          strcpy(string, "remove");
          int length2 = snprintf( NULL, 0, "%d",  nodes[idN]->conW[t]); // problems with intdup?
          char *s = malloc (length2 + 1);
          snprintf (s, length2 + 1, "%d", nodes[idN]->conW[t]);
          strcat(string, " ");
          strcat(string, strtmp);
          strcat(string, " ");
          strcat(string, id);
          strcat(string, " ");
          strcat(string, s);
          strcat(string, "\n");
          write(f, string, 10+strlen(id)+strlen(strtmp)+strlen(s));
          removeStruct(strtmp, id, s);
        }
      }
    }
  }
  removeNode(nodes, atoi(id));
  //int file = open(id, O_WRONLY);
  //write(file, "\n", 1);
  //close(file); // problems with closing named pipe?
  if(!fork()) {
    execlp("rm", "rm", id, NULL);
    perror("Erro a remover pipe\n");
  }
}

void node(int argc, char **argv) {
  char *arg[20], buf[128], *idS = argv[0];
  int r, id, i, n, f;
  if (argc<2)
    printf("Error! Not enough arguments\n"); /* argc must be >= 2 */
  else {
    id = atoi(argv[0]);
    arg[0] = strdup(argv[1]);
    for(i=1; i<argc-1; i++)
      arg[i] = strdup(argv[i+1]);
    n = i;
    //pid_t p = getpid();
    if(!(newNodeList(nodes, id, arg, n))){
      if(!fork()){
        mkfifo(idS, 0666);
        f = open(idS, O_RDONLY);
        //int fd[2];
        while ((r=(readln(f, buf, 128)))) {
          char buf2[128];
          strcpy(buf2, buf);
          char *cmd = strtok(buf2, " ");
          if (!strcmp(cmd, "remove")) {
            char *token, *arg[20];
            int narg=0;
            while(((token = strtok(NULL, " ")) != NULL))
              arg[narg++] = strdup(token);
            removeStruct(arg[0], arg[1], arg[2]);
            write(1, "REMOVENODE\n", 11);
          }
          else if (!strcmp(cmd, "change")) {
            char *token, *arg[20];
            int narg=0;
            while(((token = strtok(NULL, " ")) != NULL))
              arg[narg++] = strdup(token);
            changeComponent(nodes, atoi(arg[0]), narg, arg);
            write(1, "CHANGENODE\n", 11);
          }
          else if (!strcmp(cmd, "disconnect")) {
            char *token, *arg[20];
            int narg=0;
            while(((token = strtok(NULL, " ")) != NULL))
              arg[narg++] = strdup(token);
            disconnect(arg[0], arg[1]);
            write(1, "DISCONNECTNODE\n", 15);
          }
          else if (!strcmp(cmd, "connect")) {
            char *token, *arg[20];
            int narg=0;
            while(((token = strtok(NULL, " ")) != NULL))
              arg[narg++] = strdup(token);
            connect(narg, arg);
            write(1, "CONNECTNODE\n", 12);
          }
          else {
            int w, fd1[2], fd2[2], idN = existNode(nodes, id);
            pipe(fd1);
            pipe(fd2);
            if(!fork()){
              dup2(fd1[1],1);
              dup2(fd2[0],0);
              close(fd1[1]);
              close(fd1[0]);
              close(fd2[1]);
              close(fd2[0]);
              execvp(nodes[idN]->args[0], nodes[idN]->args);
              _exit(1);
            }
            else {
                write(fd2[1],buf,r);
                dup2(fd1[0],0);
                close(fd1[1]);
                close(fd1[0]);
                close(fd2[1]);
                close(fd2[0]);
                w = readln(0,buf2,PIPE_BUF);
            }
            printf("DENTRO NODE: %d - PID: %d\n", id, getpid());
            if (w>=r)
              writeNode(id, buf2, w);
          }
        }
        close(f);
        _exit(0);
      }
    }
  }
}

void inject (int argc, char **argv) {
  int r, f, file;
  char buf[PIPE_BUF];
  f = open(argv[0], O_WRONLY);
  if (f==-1)
    perror("Erro no inject a abrir o pipe\n");
  if (!strcmp(argv[1], "cat")) {
    file = open(argv[2], O_RDONLY);
    if (file==-1)
      perror("Erro no inject a abrir o ficheiro\n");
    while ((r=(readln(file, buf, PIPE_BUF)))) {
      strcat(buf, "\n");
      write(f, buf, r+1);
    }
    //close(file);
    write(1, "Leaving inject...\n", 18);
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
      arg[narg++] = strdup(token);
    if (!(strcmp(cmd, "node")))
      node(narg, arg);
    else if (!(strcmp(cmd, "inject")))
      inject(narg, arg);
    else if (!(strcmp(cmd, "connect"))) {
      connect(narg, arg);
      file = open(arg[0], O_WRONLY);
      if (file==-1)
        perror("Erro a abrir o pipe\n");
      strcat(buf2, "\n");
      write(f, buf2, r+1);
    }
    else if (!(strcmp(cmd, "disconnect"))) {
      disconnect(arg[0], arg[1]);
      file = open(arg[0], O_WRONLY);
      if (file==-1)
        perror("Erro a abrir o pipe\n");
      strcat(buf2, "\n");
      write(f, buf2, r+1);
    }
    else if (!(strcmp(cmd, "remove"))) {
      remov(arg[0]);
    }
    else if (!(strcmp(cmd, "change"))) {
      changeComponent(nodes, atoi(arg[0]), narg, arg);
      file = open(arg[0], O_WRONLY);
      if (file==-1)
        perror("Erro a abrir o pipe\n");
      strcat(buf2, "\n");
      write(f, buf2, r+1);
    }
  }
  close(f);
}

int main(int argc, char **argv){
  int r, narg;
  char buf[128], buf2[128], *cmd, *del=" ", *token, *arg[20];
  initNodeList(nodes);
  while((r=(readln(0, buf, 128)))) {
    strcpy(buf2, buf);
    narg = 0;
    cmd = strtok(buf, del);
    while(((token = strtok(NULL, del)) != NULL))
      arg[narg++] = strdup(token);
    if (!(strcmp(cmd, "node")))
      node(narg, arg);
    else if (!(strcmp(cmd, "inject")))
      inject(narg, arg);
    else if (!(strcmp(cmd, "rede")))
      rede(narg, arg);
    else if (!(strcmp(cmd, "connect"))) {
      connect(narg, arg);
      int f = open(arg[0], O_WRONLY);
      if (f==-1)
        perror("Erro a abrir o pipe\n");
      strcat(buf2, "\n");
      write(f, buf2, r+1);
    }
    else if (!(strcmp(cmd, "disconnect"))) {
      disconnect(arg[0], arg[1]);
      int f = open(arg[0], O_WRONLY);
      if (f==-1)
        perror("Erro a abrir o pipe\n");
      strcat(buf2, "\n");
      write(f, buf2, r+1);
    }
    else if (!(strcmp(cmd, "remove"))) {
      remov(arg[0]);
    }
    else if (!(strcmp(cmd, "change"))) {
      changeComponent(nodes, atoi(arg[0]), narg, arg);
      int f = open(arg[0], O_WRONLY);
      if (f==-1)
        perror("Erro a abrir o pipe\n");
      strcat(buf2, "\n");
      write(f, buf2, r+1);
    }
    printf("PID: %d\n", getpid());
  }
  for(r=0; nodes[r]!=NULL; r++){
    int length = snprintf( NULL, 0, "%d",  nodes[r]->id);
    char *strtmp = malloc (length + 1);
    snprintf (strtmp, length + 1, "%d", nodes[r]->id);
    int f = open(strtmp, O_WRONLY);
    if (f==-1)
      perror("Erro a abrir o pipe\n");
    //write(f, "\n", 1);
    close(f);
    if(!fork()) {
      execlp("rm", "rm", strtmp, NULL);
      perror("Erro a remover pipe\n");
    }
  }
  return 0;
}
