// gcc shell.c ./lib/linenoise.c -o shell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "./lib/linenoise.h"

#define PROMPT "$ "
#define HISTORY_LENGTH 1024
#define MAX_ARGS 1024 
#define TOKEN_SEP " \t"
#define PATH_MAX 4096

char CWD[PATH_MAX];

int s_read(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, TOKEN_SEP);
    while (token != NULL && i < (MAX_ARGS - 1)) {
        args[i++] = token;
        token = strtok(NULL, " \t");
    }
    args[i] = NULL;
    return i;
}

int s_execute(char *cmd, char **cmd_args) {
  fprintf(stdout, "Executing `%s`!\n", cmd);

  int status;
  pid_t pid;

  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Could not execute!\n");
    return -1;
  }

  if (pid == 0) {
    execvp(cmd, cmd_args);
  } else {
    // father: wait for kiddo
    if (waitpid (pid, &status, 0) != pid) {
      fprintf(stderr, "Could not wait for kiddo!\n");
      return -1;
    }
  }

  return status;  
}

typedef enum Builtin {
  CD,
  PWD,
  INVALID
} Builtin;

void builtin_impl_cd(char **args, size_t n_args);
void builtin_impl_pwd(char **args, size_t n_args);

void (*BUILTIN_TABLE[]) (char **args, size_t n_args) = {
  [CD] = builtin_impl_cd,
  [PWD] = builtin_impl_pwd,
};

Builtin builtin_code(char *cmd) {
  if (!strncmp(cmd, "cd", 2)) {
    return CD;
  } else if (!strncmp(cmd, "pwd", 3)) {
    return PWD;
  } else {
    return INVALID;
  }
}

int is_builtin(char *cmd) {
  return builtin_code(cmd) != INVALID;
}

void s_execute_builtin(char *cmd, char **args, size_t n_args) {
  BUILTIN_TABLE[builtin_code(cmd)](args, n_args);
}

void refresh_cwd(void) {
  if (getcwd(CWD, sizeof(CWD)) == NULL) {
    fprintf(stderr, "Error: Could not read working dir");
    exit(1);
  }
}

void builtin_impl_cd(char **args, size_t n_args) {
  char *new_dir = *args;
  if (chdir(new_dir) != 0) {
    fprintf(stderr, "Error: Could not change directory");
    exit(1);
  }
  refresh_cwd();
}

void builtin_impl_pwd(char **args, size_t n_args) {
  fprintf(stdout, "%s\n", CWD);
}

int main(void) {
  refresh_cwd();
  
  if (!linenoiseHistorySetMaxLen(HISTORY_LENGTH)) {
    fprintf(stderr, "Could not set linenoise history!");
    exit(1);    
  }

  char *line;
  char *args[MAX_ARGS];
  while((line = linenoise(PROMPT)) != NULL) {

    // read step
    int args_read = s_read(line, args);

    fprintf(stdout, "Read %d args\n", args_read);
    for (int i = 0; i < args_read; i++) {
      fprintf(stdout, "arg[%d] = %s\n", i, args[i]);
    }

    // skip empty lines
    if (args_read == 0) {
      linenoiseFree(line);
      continue;
    }

    char *cmd = args[0];
    char **cmd_args = args;
    if (is_builtin(cmd)) {
      // make sure builtin functions see as the first argument the
      // real first argument, and not the name of the builtin being
      // called
      s_execute_builtin(cmd, (cmd_args+1), args_read-1);
    } else {
      s_execute(cmd, cmd_args);
    }

    linenoiseHistoryAdd(line);
    linenoiseFree(line);    
  }

  return 0;
}
