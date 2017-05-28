#include "struct.h"

/* This function creates a new list of nodes */
void initNodeList(struct node **node){
  for(int i=0; i<N_EVENTS; i++)
    node[i] = NULL;
}

/* Create a new node with id, cmd, args, n and pid and append it to our array of nodes*/
void newNodeList(struct node **node, int id, char *cmd, char **args, int n, pid_t pid){
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
  node[j] = aux;
}
