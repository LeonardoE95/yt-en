#include <stdio.h>
#include <stdlib.h>

// Code used to show how to trigger a simple LCG type of behaior using
// rand(). By default the libc uses a more complex Additive Feedback
// Generator PRNG, but it also implements a simpler LCG. To trigger
// the LCG behavior we have to initialize the state with an array of
// bytes whos length is less than 32. This can be done with a
// combination of calls to initstate() and setstate().
//
// To compile:
// gcc example_3_rand_lcg.c -o example_3_rand_lcg

int main(void) {
  // initialize LCG
  char state1[8];             // !
  initstate(1337, state1, 0); // !
  setstate(state1);           // !
  
  // use the PRNG
  srand(42);
  int n = 10;
  for (int i = 0; i < n; i++) {
    printf("%d\n", rand());
  }
  return 0;
}
