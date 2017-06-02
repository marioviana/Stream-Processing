#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define PIPE_BUF 100   /* Maximum length of event */

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

/* Window component: window <column> <operation> <lines> */
void window(char *col, char *op, char *lines){
  int result, a, i, r, loop=0, before[atoi(lines)];
  for (i=0; i<atoi(lines); i++)
    before[i] = 0;
  char buf[PIPE_BUF+1], bufcpy[PIPE_BUF+1], *del=":", *token;
  while((r=(readln(0, buf, PIPE_BUF)))) {
    int x, count;
    char *buf2 = strdup(buf);
    for (x=0, count=0; buf2[x]; x++)
      count += (buf2[x] == ':');
    if (count < atoi(col)-1) {
      write(1, "\n", 1);
    }
    else {
      if (r==PIPE_BUF || ((r+2) > PIPE_BUF)) {
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
}

int main(int argc, char *argv[]) {
  window(argv[1], argv[2], argv[3]);
  return 0;
}
