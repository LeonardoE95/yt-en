#include <stdio.h>
#include <stdlib.h>

int main(void) {
  // setup simple LCG
  char state1[8];
  initstate(1337, state1, 0);
  setstate(state1);
  
  srand(42);
  
  long period = 0;
  int first = rand();

  // keep going until it repeats
  while(first != rand()) { period++; }

  printf("period=%ld\n", period);  
}
