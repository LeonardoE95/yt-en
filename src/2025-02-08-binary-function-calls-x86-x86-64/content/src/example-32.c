// Compilation:
// gcc -g -m32 example-32.c -o example-32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vulnerable(char *input) {
  char buffer[512];
  strcpy(buffer, input);  
}

void main(int argc, char *argv[]) {
  if (argc > 1) {
    vulnerable(argv[1]);
  }  
}
