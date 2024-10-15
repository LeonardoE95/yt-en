// Cross-compile with:
//   x86_64-w64-mingw32-g++ hello.c -static -o hello

#include <stdio.h>

int main(void) {
  printf("Hello World!\n");
  return 0;
}
