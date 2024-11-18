#include <stdio.h>
#include <windows.h>

typedef int (*AddNumbersFunc)(int, int);

int main() {
    HINSTANCE hinstLib;
    AddNumbersFunc addNumbers;
    BOOL fFreeResult;
    int result;

    // Load the DLL
    hinstLib = LoadLibrary(TEXT("lib.dll"));
    if (hinstLib == NULL) {
        printf("Could not load the DLL\n");
        return 1;
    }

    // Get function pointer
    addNumbers = (AddNumbersFunc)GetProcAddress(hinstLib, "add_numbers");
    if (addNumbers == NULL) {
        printf("Could not find the function in the DLL\n");
        FreeLibrary(hinstLib);
        return 1;
    }

    // Call the function
    result = addNumbers(5, 10);
    printf("Result of add_numbers(5, 10): %d\n", result);

    // Free the DLL module
    fFreeResult = FreeLibrary(hinstLib);

    return 0;
}
