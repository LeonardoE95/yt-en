#include <stdio.h>
#include <stdint.h>

// custom LCG parameters
uint32_t a = 2147483629;
uint32_t b = 2147483587;
uint32_t c = 2147483647;

uint32_t state;

uint32_t myrand(void) 
{
  uint32_t val = (((state * a) % c )+ b) % c;
  state = val;
  return val;
}

void mysrand(uint32_t seed)
{
  state = seed;
}

int main(void) {
  mysrand(1337);
  int n = 10;
  for (int i = 0; i < n; i++) {
    printf("%d\n", myrand());
  }

  return 0;
}
