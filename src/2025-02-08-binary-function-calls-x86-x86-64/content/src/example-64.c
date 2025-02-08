// Compilation:
// gcc -g  example-64.c -o example-64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vulnerable(char *input) {
  char buffer[512];
  strcpy(buffer, input);  
}

void main(int argc, char *argv[]) {
  if (argc > 1) {
    vulnerable(argv[2]);
  }
}
