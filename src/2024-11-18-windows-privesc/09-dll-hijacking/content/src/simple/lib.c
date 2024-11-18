#include <windows.h>

__declspec(dllexport) int add_numbers(int a, int b) {
    return a + b;
}
