#include "struct.h"

/* This function creates a new list of nodes */
void initNodeList(struct node **node){
  for(int i=0; i<N_EVENTS; i++)
    node[i] = NULL;
}

int existNode(struct node **node, int id){
  int j, r=-1;
  for(j=0; node[j] != NULL; j++){
    if (node[j]->id == id) {
      r = j;
      break;
    }
  }
  return r;
}

/* Create a new node with id, cmd, args, n and pid and append it to our array of nodes*/
int newNodeList(struct node **node, int id, char **arg, int n){
  if (existNode(node, id)==-1){
    int j,i;
    for(j=0; node[j] != NULL; j++){}
    NodeList aux = (NodeList) malloc (sizeof(struct node));
    aux->id = id;
    for(i=0; i<20; i++){
      aux->args[i] = NULL;
    }
    aux->args[0] = strdup("./");
    strcat(aux->args[0], strdup(arg[0]));
    for(i=1; i<n; i++)
      aux->args[i] = strdup(arg[i]);
    aux->nargs = n;
    for(i=0; i<20; i++)
      aux->conW[i] = 0;
    aux->nconW = 0;
    node[j] = aux;
    return 0;
  }
  else {
    return 1;
  }
}

void removeNode(struct node **node, int id){
  int i, j, idN = existNode(node, id);
  for(i = idN; node[i] != NULL && node[i+1] != NULL; i++) {
    node[i]->id = node[i+1]->id;
    node[i]->nargs = node[i+1]->nargs;
    node[i]->nconW = node[i+1]->nconW;
    for(j=0; node[i+1]->args[j]!=NULL; j++){
      node[i]->args[j] = strdup(node[i+1]->args[j]);
    }
    for(;j<20;j++){
      node[i]->args[j] = NULL;
    }
    for(j=0; node[i+1]->conW[j]!=0 ; j++){
      node[i]->conW[j] = node[i+1]->conW[j];
    }
    for(;j<20;j++){
      node[i]->conW[j] = 0;
    }
  }
  node[i] = NULL;
}

void changeComponent(struct node **node, int id, int narg, char **arg){
  char *aux = strdup("./");
  strcat(aux, arg[1]);
  int i, idN = existNode(node, id);
  for(i=0; i<20; i++)
    node[idN]->args[i] = NULL;
  node[idN]->args[0] = strdup(aux);
  for(i=1; arg[i+1] != NULL; i++)
    node[idN]->args[i] = strdup(arg[i+1]);
  node[idN]->nargs=narg-1;
}
