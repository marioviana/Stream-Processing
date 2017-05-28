#ifndef COMPONENTS_H
#define COMPONENTS_H

#define PIPE_BUF 50   /* Maximum length of event */

int avg(int inteiro[], int size);
int sum(int inteiro[], int size);
int max(int inteiro[], int size);
int min(int inteiro[], int size);
void remove_element(char **array, int index, int array_length);
ssize_t readln(int fld, char *buf, size_t nbyte);
void cons(char *value);
void window(char *col, char *op, char *lines);
void filter(char *value1, char *operation, char *value2);
void grep(char *col, char *search);
void spawn(int argc, char **argv);


#endif
