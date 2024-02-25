// Simple CLI interface for showcasing the Enigma header-only library

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define ENIGMA_IMPLEMENTATION
#include "../enigma.h"

#define MAX_ARGS 128

// ----------------------------------------

typedef enum {
  C_QUIT = 0,  
  C_HELP,
  C_INFO,
  C_SET,
  C_ENCRYPT,
  C_DECRYPT,
  C_UNKNOWN
} CommandType;

typedef struct {
  CommandType type;
  char *args[MAX_ARGS];
  size_t n_args;
} Command;

CommandType string2command_type(const char *cmd);
Command parse_args(char *buff);

void execute_quit(char **args, size_t n_args);
void execute_help(char **args, size_t n_args);
void execute_info(char **args, size_t n_args);
void execute_set(char **args, size_t n_args);
void execute_encrypt(char **args, size_t n_args);
void execute_decrypt(char **args, size_t n_args);

// ----------------------------------------

Enigma *ENIGMA;

// NOTE: Be careful, because the specific index of these functions
// DOES MATTER. There is a matching 1-to-1 between the CommandType
// enum and the index of these function.
void (*CLI_ACTION_TABLE[]) (char **args, size_t n_args) = {
  [C_QUIT] = execute_quit,
  [C_HELP] = execute_help,
  [C_INFO] = execute_info,
  [C_SET] = execute_set,
  [C_ENCRYPT] = execute_encrypt,
  [C_DECRYPT] = execute_decrypt,
};

// ----------------------------------------

CommandType string2command_type(const char *cmd) {
  if (strncmp(cmd, "help", 4) == 0) {
    return C_HELP;
  } else if (strncmp(cmd, "quit", 4) == 0) {
    return C_QUIT;
  } else if (strncmp(cmd, "info", 4) == 0) {
    return C_INFO;
  } else if (strncmp(cmd, "set", 3) == 0) {
    return C_SET;
  } else if (strncmp(cmd, "encrypt", 7) == 0) {
    return C_ENCRYPT;
  } else if (strncmp(cmd, "decrypt", 7) == 0) {
    return C_DECRYPT;
  } else {
    return C_UNKNOWN;
  }
}

Command parse_args(char *buff) {
  Command c;
  
  char *cmd_str = strtok(buff, " ");
  c.type = string2command_type(cmd_str);
  
  size_t i = 0;
  do {
    c.args[i++] = strtok(NULL, " ");
  } while ((c.args[i-1] != NULL) && (i < MAX_ARGS));
  c.n_args = i - 1;
  
  return c;
}

// ----------------------------------------

void execute_cmd(Command cmd) {
  CLI_ACTION_TABLE[cmd.type](cmd.args, cmd.n_args);
}

void execute_quit(char **args, size_t n_args) {
  printf("Enigma> About to exit...\n");
  exit(0);
}

void execute_help(char **args, size_t n_args) {
  printf("Enigma> List of commands...\n\n");
  printf("        quit – exit from enigma cli\n");
  printf("        help – list available commands\n");
  printf("        info – print enigma configuration\n");
  printf("        set <rotors> | set <reflector> | set <plugboard> – change enigma configuration\n");
  printf("        encrypt <plaintext> – encrypt plaintext\n");
  printf("        decrypt <ciphertext> – decrypt ciphertext\n");
  printf("\n");  
}

void execute_info(char **args, size_t n_args) {
  printf("Enigma> Current configuration...\n");
  // rotors
  printf("        Rotors (from left to right): %s, %s, %s\n",
	 ENIGMA->rotors[2].name,
	 ENIGMA->rotors[1].name,
	 ENIGMA->rotors[0].name
	 );
  printf("               Position: %d, %d, %d\n",
	 ENIGMA->rotors[2].position,
	 ENIGMA->rotors[1].position,
	 ENIGMA->rotors[0].position
	 );
  printf("                   Ring: %d, %d, %d\n",
	 ENIGMA->rotors[2].ring,
	 ENIGMA->rotors[1].ring,
	 ENIGMA->rotors[0].ring
	 );    
  
  // reflector
  printf("        Reflector: %s\n", ENIGMA->reflector.name);

  // plugboard
  printf("        Plugboard: %ld plugs\n", ENIGMA->plugboard.board_size);

  for (size_t i = 0; i < ENIGMA->plugboard.board_size; i++) {
    printf("                   (%c, %c)\n",
	   CODE2CHAR(ENIGMA->plugboard.board[i][0]),
	   CODE2CHAR(ENIGMA->plugboard.board[i][1])
	   );
  }
  
  return;
}

void set_rotor(char **args, size_t n_args) {
  // set rotor <rotor_name> <position> <ring>
  if (n_args < 4) {
    printf("Enigma> set rotor requires at least 4 args: set rotor <rotor_index> <rotor_name> <position> <ring>!\n");
    return;
  }
  
  char *rotor_index_to_swap = *args++;
  n_args--;
  char *rotor_name = *args++;
  n_args--;
  char *pos = *args++;
  n_args--;
  char *ring = *args++;
  n_args--;

  uint8_t rotor_index, rotor_position, rotor_ring;
  if (strncmp(rotor_index_to_swap, "left", 4) == 0) {
    rotor_index = 2;
  } else if (strncmp(rotor_index_to_swap, "middle", 6) == 0) {
    rotor_index = 1;
  } else if (strncmp(rotor_index_to_swap, "right", 5) == 0) {
    rotor_index = 0;
  } else {
    printf("Enigma> <rotor_index> must be either: left | middle | right \n");
    return;
  }

  // TODO: add more checks here?
  rotor_position = atoi(pos);
  rotor_ring = atoi(ring);
  
  if ((rotor_position < 0 || rotor_position > 25) && (rotor_ring < 0 || rotor_ring > 25)) {
    printf("Enigma> rotor position and ring settings must be positive integers between 0 and 25!\n");
    return;      
  }

  // DEBUG
  // printf("rotor_name = %s, len(rotor_name) = %ld, rotor_index = %d, rotor_position = %d, rotor_ring = %d\n",
  // 	   rotor_name, strlen(rotor_name), rotor_index, rotor_position, rotor_ring);

  init_rotor(&ENIGMA->rotors[rotor_index], rotor_name, rotor_position, rotor_ring);
}

void set_reflector(char **args, size_t n_args) {
  // set reflector <reflector_name>
  if (n_args < 1) {
    printf("Enigma> set reflector requires at least 1 args: set reflector <reflector_name>!\n");
    return;
  }
  
  char *reflector_name = *args++;
  n_args--;
  
  init_reflector(ENIGMA, reflector_name);
}

void set_plugboard(char **args, size_t n_args) {
  // set plugboard <k1-v1> <k2-v2> <k3-v3>    
  if ((n_args < 0) || (n_args > 11)) {
    printf("Enigma> set reflector requires at least 1 args and no more than 10 args: set reflector <L1-L2> <L3-L4> ...!\n");
    return;
  }

  size_t board_size = n_args;
  uint8_t board[board_size][2];
  for(size_t i = 0; i < n_args; i++) {
    char *l1 = strtok(*args++, "-");
    char *l2 = strtok(NULL, "-");
    board[i][0] = l1[0];
    board[i][1] = l2[0];
  }

  init_plugboard(ENIGMA, board, board_size);
}


void execute_set(char **args, size_t n_args) {
  if (n_args < 1) {
    printf("Enigma> set requires at least 1 arg!\n");
    return;
  }

  char *set_type = *args++;
  n_args--;
  
  if (strncmp(set_type, "rotor", 5) == 0) {
    set_rotor(args, n_args);
  } else if (strncmp(set_type, "reflector", 9) == 0) {
    set_reflector(args, n_args);
  } else if (strncmp(set_type, "plugboard", 9) == 0) {
    set_plugboard(args, n_args);
  } else {
    printf("Enigma> set accepts only the following args: rotor | reflector | plugboard\n");
    return;    
  }
}

void execute_encrypt(char **args, size_t n_args) {
  if (n_args != 1) {
    printf("Enigma> encrypt requires only 1 arg, the plaintext to encrypt!\n");
    return;
  }

  char *plaintext = *args++;
  n_args--;
  size_t plaintext_length = strlen(plaintext);
  
  char ciphertext[plaintext_length];
  memcpy(ciphertext, plaintext, plaintext_length + 1);
  
  enigma_encrypt(ENIGMA, plaintext, plaintext_length, ciphertext);
  printf("%s\n", ciphertext);
}

void execute_decrypt(char **args, size_t n_args) {
  if (n_args != 1) {
    printf("Enigma> decrypt requires only 1 arg, the ciphertext to encrypt!\n");
    return;
  }
  
  char *ciphertext = *args++;
  n_args--;
  size_t ciphertext_length = strlen(ciphertext);

  char plaintext[ciphertext_length];
  memcpy(plaintext, ciphertext, ciphertext_length + 1);  

  enigma_decrypt(ENIGMA, ciphertext, ciphertext_length, plaintext);
  printf("%s\n", plaintext);
}

// ----------------------------------------

int main(void) {
  // default enigma
  ENIGMA = init_enigma((const char *[]){"M3-II", "M3-I", "M3-III"},   // rotors_names
		       (const uint8_t [ROTORS_N]) {0, 0, 0}, // rotor_positions
		       (const uint8_t [ROTORS_N]) {0, 0, 0}, // rotor_ring_settings			
		       "M3-B",                                  // reflector
		       (uint8_t [][2]){                      // plugboard switches
			 {'A', 'M'}, {'F', 'I'},
			 {'N', 'V'}, {'P', 'S'},
			 {'T', 'U'}, {'W', 'Z'},			   
		       },
		       6                                      // plugboard size
		       );
  
  // REPL
  int done = 0;  
  while (!done) {
    char *buff;
    buff = readline("Enigma> ");

    if (buff == NULL) {
      printf("Enigma> Terminating process...\n");
      exit(0);
    }

    Command cmd = parse_args(buff);
    if (cmd.type == C_UNKNOWN) {
      printf("Enigma> Unknown command...type 'help' for more documentation\n");
    } else {
      execute_cmd(cmd);
    }

    // NOTE: Is it okay if in parse_args() we call the function
    // strtok() on the buffer allocated by readline() or will this
    // cause problems with memory management? can we just free it as
    // usual?
    free(buff);
  }

  destroy_enigma(ENIGMA);
  
  return 0;
}
