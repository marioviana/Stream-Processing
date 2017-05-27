#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define PIPE_BUF 50   /* Maximum length of event */
#define N_EVENTS 200  /* Maximum number of nodes in the struct */

/* Struct to save data regarding the nodes */
typedef struct node {
  int id;         /* id/number of the node */
  char *cmd;      /* the name of the command to execute in this node */
  char *args[20]; /* the arguments of the command, with 10 being the max number of arguments */
  int nargs;      /* number of arguments in args */
  pid_t pid;      /* pid of the process */
  int conR[20];
  int nconR;
  int conW[20];
  int nconW;
} *NodeList;

/* declaration of the global structure for nodes */
NodeList nodes[N_EVENTS];

/* This function creates a new list of nodes */
void initNodeList(){
  for(int i=0; i<N_EVENTS; i++)
    nodes[i] = NULL;
}

/* Create a new node with id, cmd, args, n and pid and append it to our array of nodes*/
void newNodeList(int id, char *cmd, char **args, int n, pid_t pid){
  int j,i;
  //for(j=0; nodes[j] != NULL; j++){}
  j = id-1;
  NodeList aux = (NodeList) malloc (sizeof(struct node));
  aux->id = id;
  aux->cmd = strdup(cmd);
  for(i=0; i<n; i++)
    aux->args[i] = strdup(args[i]);
  aux->nargs = n;
  aux->pid = pid;
  for(i=0; i<20; i++) {
    aux->conR[i] = 0;
    aux->conW[i] = 0;
  }
  aux->nconR = 0;
  aux->nconW = 0;
  nodes[j] = aux;
}

/* Auxilixar function to calculate average of int array for window function */
int avg(int inteiro[], int size){
  int i, total=0;
  for(i=0; i<size; i++)
    total += inteiro[i];
  if (size==0)
    return 0;
  else
    return (total/size);
}

/* Auxilixar function to calculate sum of int array for window function */
int sum(int inteiro[], int size){
  int i, total=0;
  for(i=0; i<size; i++)
    total += inteiro[i];
  if (size==0)
    return 0;
  else
    return total;
}

/* Auxilixar function to calculate max of int array for window function */
int max(int inteiro[], int size){
  int i, max=inteiro[0];
  for(i=1; i<size; i++)
    if (inteiro[i]>max)
      max = inteiro[i];
  if (size==0)
    return 0;
  else
    return max;
}

/* Auxilixar function to calculate min of int array for window function */
int min(int inteiro[], int size){
  int i, min=inteiro[0];
  for(i=1; i<size; i++)
    if (inteiro[i]<min)
      min = inteiro[i];
  if (size==0)
    return 0;
  else
    return min;
}

/* Auxiliar function to remove an element from an array for spawn function */
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

/* Const component: const <value> */
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

/* Window component: window <column> <operation> <lines> */
void window(char *col, char *op, char *lines){
  int result, a, i, r, loop=0, before[atoi(lines)];
  for (i=0; i<atoi(lines); i++)
    before[i] = 0;
  char buf[PIPE_BUF+1], bufcpy[PIPE_BUF+1], *del=":", *token;
  while((r=(readln(0, buf, PIPE_BUF)))) {
    if (r==PIPE_BUF || ((r+3) > PIPE_BUF)) {
      write(1, buf, PIPE_BUF);
      write(1, "\n", 1);
    }
    else {
      if (loop < (atoi(lines)))
        i = loop;
      else
        i = (atoi(lines));
      if (!strcmp(op, "avg")){
        result = avg(before, i);
        strcat(buf, ":");
        int length = snprintf( NULL, 0, "%d", result );
        char* resultS = malloc( length + 1 );
        snprintf( resultS, length + 1, "%d", result );
        strcat(buf, resultS);
        write(1, buf, r+1+strlen(resultS));
        write(1, "\n", 1);
        free(resultS);
      }
      else if (!strcmp(op, "sum")){
        result = sum(before, i);
        strcat(buf, ":");
        int length = snprintf( NULL, 0, "%d", result );
        char* resultS = malloc( length + 1 );
        snprintf( resultS, length + 1, "%d", result );
        printf("%s\n", resultS);
        strcat(buf, resultS);
        write(1, buf, r+1+strlen(resultS));
        write(1, "\n", 1);
        free(resultS);
      }
      else if (!strcmp(op, "max")){
        result = max(before, i);
        strcat(buf, ":");
        int length = snprintf( NULL, 0, "%d", result );
        char* resultS = malloc( length + 1 );
        snprintf( resultS, length + 1, "%d", result );
        strcat(buf, resultS);
        write(1, buf, r+1+strlen(resultS));
        write(1, "\n", 1);
        free(resultS);
      }
      else if (!strcmp(op, "min")){
        result = min(before, i);
        strcat(buf, ":");
        int length = snprintf( NULL, 0, "%d", result );
        char* resultS = malloc( length + 1 );
        snprintf( resultS, length + 1, "%d", result );
        strcat(buf, resultS);
        write(1, buf, r+1+strlen(resultS));
        write(1, "\n", 1);
        free(resultS);
      }
      a=1;
      strcpy(bufcpy, buf);
      token = strtok(bufcpy, del);
      if (atoi(col)==1)
        before[loop%(atoi(lines))] = atoi(token);
      else {
        while(((token = strtok(NULL, del)) != NULL) && (a+1)<atoi(col)) {
          a++;
        }
        if (atoi(col)==(a+1))
          before[loop%(atoi(lines))] = atoi(token);
        else
          before[loop%(atoi(lines))] = 0;
      }
    }
    loop++;
  }
}

/* Filter component: filter <column> <operation> <integer> */
void filter(char *value1, char *operation, char *value2) {
  int i, r, v1, v2, v3, c1, c2;
  char buf[PIPE_BUF+1], *aux[PIPE_BUF+1], bufcpy[PIPE_BUF+1];
  v1 = atoi(value1);
  v2 = atoi(value2);
  v3 = v1>v2?v1:v2; // v3 = max v1 v2

  while((r=(readln(0, buf, PIPE_BUF)))) {
    strcpy(bufcpy, buf);
    aux[0] = strtok(bufcpy, ":");
    i=1;
    while(i<=(v3-1)){
      aux[i] = strtok(NULL, ":");
      i++;
    }
    if(!(strcmp(operation, "="))){
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
    }
  }
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
    if (!fork()) {
      execvp(argv[1], argv+1);
      exit(1);
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

void writeNode(int id, char buf[128], int r) {
  int nconW = nodes[id-1]->nconW;
  if (nconW>0) {
    for(int i=0; i<nconW; i++){
      if (!fork()) {
        int length = snprintf( NULL, 0, "%d",  nodes[id-1]->conW[i]);
        char *strtmp = malloc (length + 1);
        snprintf (strtmp, length + 1, "%d", nodes[id-1]->conW[i]);
        int f = open(strtmp, O_WRONLY);
        write(f, buf, r);
        write(f, "\n", 1);
        close(f);
      }
    }
    for(int i=0; i<nconW; i++){
      wait(0);
    }
  }
  else {
    write(1, buf, r);
    write(1, "\n", 1);
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
    newNodeList(id,cmd,arg,i,p);
    if(!fork()){
      mkfifo(idS, 0666);
    }
    else {
      wait(0);
    }
    if(!fork()){
      mkfifo(idS, 0666);
      f = open(idS, O_RDONLY);
      int fd[2];
      while ((r=(readln(f, buf, 128)))) {
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
        write(1, "DENTRO DO NODE ", 15);
        write(1, idS, 1);
        write(1, "\n", 1);
        writeNode(id, buf, r);
      }
    }
  }
}

void inject (int argc, char **argv) {
  int r, f, file, id = atoi(argv[0]);
  char *idS = argv[1];
  if(!fork()) {
    char *buf[PIPE_BUF];
    f = open(argv[0], O_WRONLY);
    if (!strcmp(argv[1], "cat")) {
      file = open(argv[2], O_RDONLY);
      while ((r=(readln(file, buf, PIPE_BUF)))) {
        write(f, buf, r);
        write(f, "\n", 1);
        //printf("INJECT\n");
      }
      close(file);
    }
    else {
      while ((r=(readln(0, buf, PIPE_BUF)))) {
        write(f, buf, r);
        write(f, "\n", 1);
        //printf("INJECT\n");
      }
      write(1, "Leaving inject...\n", 18);
    }
    //write(1, "SAI DO INJECT\n", 14);
    close(f);
    _exit(0);
  }
  else {
    wait(0);
  }
}

void connect(int argc, char **argv){
  int l, id = atoi(argv[0]);
  for(l=1; l<argc; l++) {
    nodes[id-1]->conW[nodes[id-1]->nconW] = atoi(argv[l]);
    nodes[id-1]->nconW = nodes[id-1]->nconW+1;
  }
  /*for(l=1; l<argc; l++) {
    id = atoi(argv[l]);
    nodes[id-1]->conR[nodes[id-1]->nconR] = atoi(argv[0]);
    nodes[id-1]->nconR++;
  }*/
}

void funout(int argc, char **argv){
  for(int i=2; i<argc; i++) {
    int pd[2];
    pipe(pd);
    if(!fork()) {
      close(pd[0]);
      dup2(pd[1],1);
      close(pd[1]);
      close(pd[0]);
      execlp(argv[1], argv[1], NULL);
      _exit(1);
    }
    if(!fork()) {
      dup2(pd[0],0);
      close(pd[0]);
      close(pd[1]);
      execlp(argv[i], argv[i], NULL);
      _exit(1);
    }
  }
}

void funin(int argc, char **argv){
  for(int i=1; i<argc-1; i++) {
    int pd[2];
    pipe(pd);
    if(!fork()) {
      dup2(pd[1],1);
      close(pd[1]);
      close(pd[0]);
      execlp(argv[i], argv[i], NULL);
    }
    if(!fork()) {
      dup2(pd[0],0);
      close(pd[0]);
      close(pd[1]);
      execlp(argv[argc-1], argv[argc-1], NULL);
    }
  }
}

void rede(int argc, char **argv){
  int r, f = open(argv[0], O_RDONLY), narg;
  char buf[128], *cmd, *del=" ", *token, *arg[20];
  while ((r=(readln(f, buf, 128)))) {
    narg = 0;
    cmd = strtok(buf, del);
    while(((token = strtok(NULL, del)) != NULL))
      arg[narg++] = token;
    if (!(strcmp(cmd, "node")))
      node(narg, arg);
    else if (!(strcmp(cmd, "inject")))
      inject(narg, arg);
    else if (!(strcmp(cmd, "connect")))
      connect(narg, arg);
  }
  close(f);
}

int main(int argc, char **argv){
  int r, narg;
  char buf[128], *cmd, *del=" ", *token, *arg[20];
  initNodeList();
  while((r=(readln(0, buf, 128)))) {
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
    else if (!(strcmp(cmd, "connect")))
      connect(narg, arg);
  }
  return 0;
}
