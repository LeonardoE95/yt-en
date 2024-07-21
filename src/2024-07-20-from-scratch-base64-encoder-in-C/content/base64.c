/*
  Simple Base64 encoder implemented in C.

  Author: Leonardo Tamiano.

  Made for the following youtube video:
  - https://youtu.be/u2bevjdRAFk
  
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>

// -----------------------------------------------------

// Function signatures

void base64_encode(const char *input, const unsigned long input_length, char *output);
void base64_decode(const char *input, const unsigned long input_length, char *output);

// -----------------------------------------------------

// Core implementation

char *BASE64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// This table implements the mapping from 8-bit ascii value to 6-bit
// base64 value and it is used during the base64 decoding
// process. Since not all 8-bit values are used, some of them are
// mapped to -1, meaning that there is no 6-bit value associated with
// that 8-bit value.
//
int UNBASE64[] = {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0-11
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 12-23
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 24-35
  -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, // 36-47
  52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -2, // 48-59
  -1,  0, -1, -1, -1,  0,  1,  2,  3,  4,  5,  6, // 60-71
   7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, // 72-83
  19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 84-95
  -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, // 96-107
  37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, // 108-119
  49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 120-131
};

void base64_encode(const char *input, const unsigned long input_length, char *output) {
  int input_index = 0;
  int output_index = 0;

  while (input_index < input_length) {
    // in each iteration we process 24 bits of the input. That is, we
    // process 3 bytes

    // process first 6-bits
    output[output_index++] = BASE64[(input[input_index] & 0xFC) >> 2];

    // handle padding case n.1
    if ((input_index + 1) == input_length) {
      // remainin bits that needs to be processed are the right-most 2
      // bit of the last byte of input, and we also need to add two
      // bytes of padding
      output[output_index++] = BASE64[(input[input_index] & 0x03) << 4];
      output[output_index++] = '=';
      output[output_index++] = '=';
      break;
    }

    // process second 6-bits
    output[output_index++] = BASE64[ ((input[input_index] & 0x3) << 4) | ((input[input_index + 1] & 0xF0) >> 4) ];

    // handle padding case n.2
    if ((input_index + 1) == input_length) {
      // remainin bits that needs to be processed are the right-most 4
      // bits of the last byte of input, and we also need to a single
      // byte of padding.
      output[output_index++] = BASE64[(input[input_index + 1] & 0x0F) << 2];
      output[output_index++] = '=';
      break;
    }

    // process third 6-bits
    output[output_index++] = BASE64[ ((input[input_index + 1] & 0x0F) << 2) | ((input[input_index + 2] & 0xC0) >> 6) ];    

    // process fourth 6-bits
    output[output_index++] = BASE64[(input[input_index + 2] & 0x3F)];

    input_index += 3;
  }

  output[output_index] = '\0';
  return;
}


void base64_decode(const char *input, const unsigned long input_length, char *output) {
  int input_index = 0;
  int output_index = 0;  

  // check if the input length is an even multiple of 4
  if (input_length & 0x03) {
    fprintf(stderr, "[ERROR]: Invalid base64 input, length not even multiple of 4\n");
    return;
  }

  while (input_index < input_length) {
    char first, second, third, fourth;

    // make sure that the next 4 bytes of input are properly encoded
    // base64 data
    for (int i = 0; i < 4; i++) {
      int value = input[input_index + i];
      if ((value < 0) || (value > 131) || (UNBASE64[value] == -1)) {
	fprintf(stderr, "[ERROR]: Invalid base64 character, cannot decode: %c\n", value);
	return;
      }
    }
    
    first = UNBASE64[(int) input[input_index]];
    second = UNBASE64[(int) input[input_index + 1]];
    third = UNBASE64[(int) input[input_index + 2]];
    fourth = UNBASE64[(int) input[input_index + 3]];        

    // reconstruct first byte
    output[output_index++] = (first << 2) | ((second & 0x30) >> 4);

    // reconstruct second byte
    if (input[input_index + 2] != '=') {
      output[output_index++] = ((second & 0xF) << 4) | ((third & 0x3C) >> 2);
    }

    // reconstruct third byte
    if (input[input_index + 3] != '=') {
      output[output_index++] = ((third & 0x03) << 6) | fourth;
    }

    input_index += 4;
  }

  output[output_index++] = '\0';
  return;
}

// -----------------------------------------------------

int main(int argc, char **argv) {
  bool decode_mode = false; // -d

  if (argc > 2) {
    fprintf(stdout, "[INFO]: Usage: %s [-d]\n", argv[0]);
    return 1;
  }

  // parse arguments
  if (argc == 2) {
    decode_mode = memcmp(argv[1], "-d", 2) == 0;

    if (!decode_mode) {
      fprintf(stderr, "[ERROR]: Options allowed are `-d`\n");
      return 1;
    }    
  }

  // Main loop
  while(!feof(stdin)) {
    char *input = readline("base64> ");
    unsigned long input_length = strlen(input);

    if (!input) { continue; }
    
    char *output = NULL;
    unsigned long output_length = strlen(input);

    if (decode_mode) {
      // decode
      unsigned long padding_length = ((input_length > 1) && (input[input_length - 1] == '=')) ?
	(((input_length > 2) && input[input_length - 2] == '=') ? 2 : 1) : 0;

      output_length = 3 * ceil(input_length / 4.0 ) - padding_length + 1;
      output = malloc(output_length);
      base64_decode(input, input_length, output);
    } else {
      // encode
      output_length = ceil(input_length / 3.0) * 4 + 1;
      output = malloc(output_length);
      base64_encode(input, input_length, output);
    }

    printf("%s\n", output);

    free(input);
    free(output);
  }
  
  return 0;
}
