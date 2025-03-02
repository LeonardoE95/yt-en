// Usage: gcc -ggdb -o hello hello.c

#include <stdio.h>

void functionC(int z) {
    int c_var = z * 2; // Local variable
    printf("Inside functionC: c_var = %d\n", c_var);
}

void functionB(int y) {
    int b_var = y + 10; // Local variable
    printf("Inside functionB: b_var = %d\n", b_var);
    functionC(b_var);   // Call functionC
}

void functionA(int x) {
    int a_var = x - 5; // Local variable
    printf("Inside functionA: a_var = %d\n", a_var);
    functionB(a_var);  // Call functionB
}

int recursiveFunction(int n) {
    if (n <= 0) return 1;
    int result = n * recursiveFunction(n - 1);
    printf("Returning from recursiveFunction(%d) = %d\n", n, result);
    return result;
}

int main() {
    printf("Starting main function\n");
    
    functionA(20);  // Call functionA

    int fact = recursiveFunction(5);  // Recursive function call
    printf("Factorial result: %d\n", fact);
    
    return 0;
}
