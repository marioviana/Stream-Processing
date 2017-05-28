#ifndef STRUCT_H
#define STRUCT_H

#define N_EVENTS 200

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

void initNodeList(struct node **node);
void newNodeList(struct node **node, int id, char *cmd, char **args, int n, pid_t pid);

#endif
