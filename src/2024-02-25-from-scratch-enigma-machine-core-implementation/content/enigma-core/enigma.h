#ifndef ENIGMA_H_
#define ENIGMA_H_
// header

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef size_t usize;

#define ALPHABET_SIZE 26
#define PLUGBOARD_SIZE 10
#define ROTORS_N 3
#define LABEL_LENGTH 42

// NOTE: By defining these macros in the following way we're assuming
// the input of the machine will always be capital letters of the
// english alhpabet
//
// ABCDFEGHIJKLMNOPQRSTUVWXZ
//
// If you want to also support lowercase letters you have to change
// these defines.
//
#define CHAR2CODE(ch) ((u8) ((ch) - 'A'))
#define CODE2CHAR(code) ((char) ('A' + (code)))

typedef u8 Wiring[ALPHABET_SIZE];

// ----------

typedef struct {
  u8 board[PLUGBOARD_SIZE][2];
  usize board_size;
} Plugboard;

typedef struct {
  Wiring wiring;
  char name[LABEL_LENGTH];
} Reflector;

// NOTE: For now we assume all rotors have a single
// notch. Historically this is not the case, as some rotors were
// developed with two notches.
// 
typedef struct {
  Wiring forward_wiring;  // right->left
  Wiring backward_wiring; // left->right
  u8 position;
  u8 notch;
  u8 ring;
  char name[LABEL_LENGTH];
} Rotor;

typedef enum {
  RO_FORWARD,
  RO_BACKWARD,
} RotorOrder;

typedef struct {
  Plugboard plugboard;
  Rotor rotors[ROTORS_N];
  Reflector reflector;
} Enigma;

typedef struct {
  char name[LABEL_LENGTH];
  char wiring[ALPHABET_SIZE];
  u8 notch;
} RotorModel;

typedef struct {
  char name[LABEL_LENGTH];
  char wiring[ALPHABET_SIZE];
} ReflectorModel;

// ----
// Function signatures

// Public APIs
void enigma_encrypt(Enigma *e, const char *plaintext, usize plaintext_len, char *ciphertext);
void enigma_decrypt(Enigma *e, const char *ciphertext, usize ciphertext_len, char *plaintext);

Enigma *init_enigma(const char *rotors_names[ROTORS_N],
		    const u8 rotor_positions[ROTORS_N],
		    const u8 rotor_ring_settins[ROTORS_N],
		    const char *reflector_name,
		    u8 (*plugboard)[2],
		    usize plugboard_size);

void destroy_enigma(Enigma *e);

// Internal APIs
void move_rotors(Enigma *e);

u8 apply_rotor(Rotor *r, u8 char_code, RotorOrder order);
u8 apply_rotors(Enigma *e, u8 char_code, RotorOrder order);
u8 apply_plugboard(Enigma *e, const u8 char_code);
u8 apply_reflector(Enigma *e, const u8 char_code);

void apply_enigma(Enigma *e, const u8 *input, usize input_len, u8 *output);

void init_rotors(Enigma *e, const char *rotor_names[ROTORS_N], const u8 rotor_positions[ROTORS_N], const u8 rotor_ring_settins[ROTORS_N]);
void init_rotor(Rotor *r, const char *rotor_name, const u8 position, const u8 ring);
void init_wiring(Wiring wiring, const char *alphabet, usize alphabet_len);
void reverse_wiring(Wiring new_wiring, Wiring old_wiring, usize wiring_len);
void init_plugboard(Enigma *e, u8 (*plugboard)[2], usize plugboard_size);
void init_reflector(Enigma *e, const char *reflector_name);

# endif // ENIGMA_H_

// ---------------------------------------------------

#ifdef ENIGMA_IMPLEMENTATION
// implementation

// Data regarding historical models, taken from
// https://www.cryptomuseum.com/crypto/enigma/

RotorModel KNOWN_ROTORS[] = {
  {"M3-I"  , "EKMFLGDQVZNTOWYHXUSPAIBRCJ", CHAR2CODE('Q')},
  {"M3-II" , "AJDKSIRUXBLHWTMCQGZNPYFVOE", CHAR2CODE('E')},
  {"M3-III", "BDFHJLCPRTXVZNYEIWGAKMUSQO", CHAR2CODE('V')},
  {"M3-IV" , "ESOVPZJAYQUIRHXLNFTGKDCMWB", CHAR2CODE('J')},
  {"M3-V"  , "VZBRGITYUPSDNHLXAWMJQOFECK", CHAR2CODE('Z')},  
};
usize KNOWN_ROTORS_LENGTH = (sizeof(KNOWN_ROTORS) / sizeof(RotorModel));


ReflectorModel KNOWN_REFLECTORS[] = {
  {"M3-A", "EJMZALYXVBWFCRQUONTSPIKHGD"},
  {"M3-B", "YRUHQSLDPXNGOKMIEBFZCWVJAT"},
  {"M3-C", "FVPJIAOYEDRZXWGCTKUQSBNMHL"},  
};
usize KNOWN_REFLECTORS_LENGTH = (sizeof(KNOWN_REFLECTORS) / sizeof(ReflectorModel));

// ---------------------------
// Internal APIs

void move_rotors(Enigma *e) {
  // Double stepping cause by the claw mechanism used for rotating the
  // rotors makes the second rotor move twice in a row, if the first
  // movement brings it in the turnover position during the first
  // rotation.
  // 
  // https://en.wikipedia.org/wiki/Enigma_machine
  // https://www.youtube.com/watch?v=ds8HoowfewA
  // https://www.youtube.com/watch?v=5StZlF-clPc
  // https://www.youtube.com/watch?v=hcVhQeZ5gI4
  if (e->rotors[1].position == e->rotors[1].notch) {
    e->rotors[2].position = (e->rotors[2].position + 1) % ALPHABET_SIZE;
    e->rotors[1].position = (e->rotors[1].position + 1) % ALPHABET_SIZE;
  } else if (e->rotors[0].position == e->rotors[0].notch) {
    e->rotors[1].position = (e->rotors[1].position + 1) % ALPHABET_SIZE;
  }
  e->rotors[0].position = (e->rotors[0].position + 1) % ALPHABET_SIZE;
}

u8 apply_rotor(Rotor *r, u8 char_code, RotorOrder order) {
  
  char_code = (char_code - r->ring + r->position + ALPHABET_SIZE) % ALPHABET_SIZE;
  if (order == RO_FORWARD) {
    char_code = r->forward_wiring[char_code];
  } else if (order == RO_BACKWARD) {
    char_code = r->backward_wiring[char_code];
  } else {
    assert(0 && "apply_rotors(): unreachable.");
  }
  char_code = (char_code + r->ring - r->position + ALPHABET_SIZE) % ALPHABET_SIZE;

  return char_code;
}

u8 apply_rotors(Enigma *e, u8 char_code, RotorOrder order) {
  switch(order) {
    
  case RO_FORWARD: {
    char_code = apply_rotor(&e->rotors[0], char_code, RO_FORWARD);
    char_code = apply_rotor(&e->rotors[1], char_code, RO_FORWARD);
    char_code = apply_rotor(&e->rotors[2], char_code, RO_FORWARD);    
  } break;

  case RO_BACKWARD: {
    char_code = apply_rotor(&e->rotors[2], char_code, RO_BACKWARD);
    char_code = apply_rotor(&e->rotors[1], char_code, RO_BACKWARD);
    char_code = apply_rotor(&e->rotors[0], char_code, RO_BACKWARD);        
  } break;

  default: assert(0 && "apply_rotors(): unreachable");
  }
  
  return char_code;
}

u8 apply_plugboard(Enigma *e, const u8 char_code) {
  for (usize i = 0; i < e->plugboard.board_size; i++) {
    if (char_code == e->plugboard.board[i][0]) {
      return e->plugboard.board[i][1];
    } else if (char_code == e->plugboard.board[i][1]) {
      return e->plugboard.board[i][0];
    } 
  }
  return char_code;
}

u8 apply_reflector(Enigma *e, const u8 char_code) {
  return e->reflector.wiring[char_code];
}

void apply_enigma(Enigma *e, const u8 *input, usize input_len, u8 *output) {
  // Assumes output has been alread allocated and that
  // 
  // len(output) == input_len
  //
  for(usize i = 0; i < input_len; i++) {
    u8 char_code = CHAR2CODE(input[i]);

    // Movement is executed before encryption
    move_rotors(e);

    // FORWARD PASS
    char_code = apply_plugboard(e, char_code);
    char_code = apply_rotors(e, char_code, RO_FORWARD);

    // REFLECTOR
    char_code = apply_reflector(e, char_code);

    // BACKWARD PASS
    char_code = apply_rotors(e, char_code, RO_BACKWARD);
    char_code = apply_plugboard(e, char_code);

    output[i] = CODE2CHAR(char_code);
  }
}

// Initialization functions

// NOTE: We specify rotors in the init array from left to right. Given
// however that the most frequent rotor is the right-most rotor, we
// save that on index-0.
void init_rotors(Enigma *e,
		 const char *rotor_names[ROTORS_N],
		 const u8 rotor_positions[ROTORS_N],
		 const u8 rotor_ring_settings[ROTORS_N]) {
  for (usize i = 0; i < ROTORS_N; i++) {
    init_rotor(&e->rotors[i],
	       rotor_names[ROTORS_N - 1 - i],
	       rotor_positions[ROTORS_N - 1 - i],
	       rotor_ring_settings[ROTORS_N - 1 - i]);
  }
}

void init_rotor(Rotor *r, const char *rotor_name, const u8 position, const u8 ring) {
 u8 found = 0;

  for(usize i = 0; i < KNOWN_ROTORS_LENGTH; i++) {
    if (strcmp(KNOWN_ROTORS[i].name, rotor_name) == 0) {
      found = 1;

      char *known_name = KNOWN_ROTORS[i].name;
      char *known_wiring = KNOWN_ROTORS[i].wiring;
      u8 known_notch = KNOWN_ROTORS[i].notch;

      init_wiring(r->forward_wiring, known_wiring, ALPHABET_SIZE);
      reverse_wiring(r->backward_wiring, r->forward_wiring, ALPHABET_SIZE);

      r->notch = known_notch;
      r->position = position;
      r->ring = ring;

      // copy also NULL-terminating byte
      memcpy(r->name, known_name, strlen(known_name) + 1);
    }
  }

  if(!found) {
    assert(0 && "init_rotor(): unsupported rotor\n");
  }  
}

void init_wiring(Wiring wiring, const char *alphabet, usize alphabet_len){
  for (usize i = 0; i < alphabet_len; i++) {
    wiring[i] = CHAR2CODE(alphabet[i]);
  }  
}

// Used to change the direction of old_wiring into new_wiring
//
// new_wiring[X] = Y if and only if old_wiring[Y] = X
// 
void reverse_wiring(Wiring new_wiring, Wiring old_wiring, usize wiring_len){
  for (usize i = 0; i < wiring_len; i++) {
    new_wiring[old_wiring[i]] = (u8) i;
  }
}

void init_reflector(Enigma *e, const char *reflector_name) {
  u8 found = 0;

  for(usize i = 0; i < KNOWN_REFLECTORS_LENGTH; i++) {
    if (strcmp(KNOWN_REFLECTORS[i].name, reflector_name) == 0) {
      found = 1;

      char *known_name = KNOWN_REFLECTORS[i].name;
      char *known_wiring = KNOWN_REFLECTORS[i].wiring;

      init_wiring(e->reflector.wiring, known_wiring, ALPHABET_SIZE);

      // copy also NULL-terminating byte
      memcpy(e->reflector.name, known_name, strlen(known_name) + 1);
    }
  }

  if(!found) {
    assert(0 && "init_reflector(): unsupported reflector\n");
  }

}

void init_plugboard(Enigma *e, u8 (*plugboard)[2], usize plugboard_size){
  e->plugboard.board_size = plugboard_size;
  for(usize i = 0; i < plugboard_size; i++) {
    e->plugboard.board[i][0] = CHAR2CODE(plugboard[i][0]);
    e->plugboard.board[i][1] = CHAR2CODE(plugboard[i][1]);
  }
}

// ---------------------------
// Public APIs

void enigma_encrypt(Enigma *e, const char *plaintext, usize plaintext_len, char *ciphertext) {
  assert(plaintext_len == strlen(plaintext) && "[ERROR] enigma_encrypt(): plaintext_len != strlen(plaintext)");
  assert(strlen(ciphertext) == plaintext_len && "[ERROR] enigma_encrypt(): strlen(ciphertext) != plaintext_len");
  apply_enigma(e, (const u8*) plaintext, plaintext_len, (u8*) ciphertext);
}

void enigma_decrypt(Enigma *e, const char *ciphertext, usize ciphertext_len, char *plaintext) {
  assert(ciphertext_len == strlen(ciphertext) && "[ERROR] enigma_decrypt(): ciphertext_len != strlen(ciphertext)");
  assert(strlen(plaintext) == ciphertext_len && "[ERROR] enigma_decrypt(): strlen(plaintext) != ciphertext_len");
  apply_enigma(e, (const u8*) ciphertext, ciphertext_len, (u8*) plaintext);
}

Enigma *init_enigma(const char *rotor_names[ROTORS_N],
		    const u8 rotor_positions[ROTORS_N],
		    const u8 rotor_ring_settings[ROTORS_N],
		    const char *reflector_name,
		    u8 (*plugboard)[2],
		    usize plugboard_size) {

  if (plugboard_size > PLUGBOARD_SIZE || plugboard_size < 0) {
    printf("[ERROR]: init_enigma() - Supplied plugboard size (%ld) is invalid", plugboard_size);
    exit(0);
  }

  Enigma *e = calloc(1, sizeof(Enigma));

  init_rotors(e, rotor_names, rotor_positions, rotor_ring_settings);
  init_reflector(e, reflector_name);
  init_plugboard(e, plugboard, plugboard_size);

  return e;
}

void destroy_enigma(Enigma *e) {
  if (e) {
    free(e);
  }
}

#endif // ENIGMA_IMPLEMENTATION
