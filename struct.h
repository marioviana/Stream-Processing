#ifndef STRUCT_H
#define STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define N_EVENTS 200

typedef struct node {
  int id;         /* id/number of the node */
  char *args[20]; /* the arguments of the command, with 10 being the max number of arguments */
  int nargs;      /* number of arguments in args */
  int conW[20];
  int nconW;
} *NodeList;

void initNodeList(struct node **node);
int existNode(struct node **node, int id);
int newNodeList(struct node **node, int id, char **args, int n);
void removeNode(struct node **node, int id);
void changeComponent(struct node **node, int id, int narg, char **arg);

#endif
