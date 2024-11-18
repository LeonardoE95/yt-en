// x86_64-w64-mingw32-gcc -mwindows -municode -O2 -s -o hello.exe -L. -lmalicious hello.c

#include <windows.h>
#include <stdio.h>

int wWinMain(HINSTANCE instance, HINSTANCE previnstance, LPWSTR cmdline, int showcmd) {

  /*Typedef the hello function*/
  typedef int (*pfunc)();

  /*Windows handle*/
  HANDLE hdll;

  /*A pointer to a function*/
  pfunc test;

  /*LoadLibrary*/
  hdll = LoadLibrary("malicious.dll");
  printf("Hello World!");

  // // /*GetProcAddress*/
  // test = (pfunc)GetProcAddress(hdll, "Test");

  // // /*Call the function*/
  // test();  
  
  return 0;
}
