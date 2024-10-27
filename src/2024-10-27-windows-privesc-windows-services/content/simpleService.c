// Inspired from:
// - https://gist.github.com/mmmunk/0b0adbccb6b91e778e3a6c6b47908c9c
//
// Compile:
// - x86_64-w64-mingw32-gcc -mwindows -municode -O2 -s -o simpleService.exe simpleService.c
//
// Create service:
// - sc.exe create SimpleService binPath= "C:\Users\Quickemu\Downloads\simpleService.exe"
//
// Query service:
// - 
//
// Delete the service:
// - sc.exe delete SimpleService

#include <windows.h>
#include <stdio.h>

#define SERVICE_NAME L"simpleService"

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE ServiceStatusHandle;

char TempFile[256] = { 0 };

void WriteToLog(char *s) {
  FILE *log;
  log = fopen(TempFile, "a+");
  if (log == NULL) { return; }  
  fprintf(log, "%s\n", s);
  fclose(log);
}

void ServiceControlHandler(DWORD control) {
  switch (control) {
  case SERVICE_CONTROL_PAUSE:
    ServiceStatus.dwCurrentState = SERVICE_PAUSED;
    break;
  case SERVICE_CONTROL_CONTINUE:
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    break;
  case SERVICE_CONTROL_STOP:
  case SERVICE_CONTROL_SHUTDOWN:
    ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    break;
  }
  SetServiceStatus(ServiceStatusHandle, &ServiceStatus);
}

void ServiceMain(DWORD argc, LPWSTR *argv) {
  // Setup service
  ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
  ServiceStatus.dwCurrentState = SERVICE_RUNNING;
  ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
  ServiceStatus.dwWin32ExitCode = NO_ERROR ;
  ServiceStatus.dwServiceSpecificExitCode = 0;
  ServiceStatus.dwCheckPoint = 0;
  ServiceStatus.dwWaitHint = 0;

  ServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceControlHandler);
  if (ServiceStatusHandle == 0) {
    return;
  }

  if (SetServiceStatus(ServiceStatusHandle, &ServiceStatus) == 0) {
    return;
  }

  // Init service
  GetTempPathA(sizeof TempFile, TempFile);
  strcat(TempFile, "TempLogger.log");

  // Service main loop
  while (ServiceStatus.dwCurrentState != SERVICE_STOPPED) {
    Sleep(5000);
    if (ServiceStatus.dwCurrentState == SERVICE_RUNNING) {
      WriteToLog("Please Subscribe!!");
    }
  }
}

// ------------------------------------------------------

int wWinMain(HINSTANCE instance, HINSTANCE previnstance, LPWSTR cmdline, int showcmd) {
  SERVICE_TABLE_ENTRY StartTable[] = {{SERVICE_NAME, ServiceMain}, {NULL, NULL}};
  StartServiceCtrlDispatcher(StartTable);
  return 0;
}
