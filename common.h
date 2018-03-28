#include<stdio.h>
#include<stdlib.h>

#define PORT 9011

void error(char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
