#include <stdio.h>

void hello(char *name) {
  int age = 42;
  printf("Hello %s, your age is: %d\n", name, age);
}

int main(void) {
  char *name = "Leonardo";
  hello(name);

  return 0;
}
