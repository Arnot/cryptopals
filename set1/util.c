#include "cryptopals.h"
#include <stdlib.h>
#include <stdio.h>

char *hex_to_bytes(char *input_string, unsigned int num_bytes) {
  int i;

  char *out = calloc(num_bytes,num_bytes);
  char conv_string[3] = {0};

  if (out == NULL) {
    printf("Error allocating byte-array\n");
    return NULL;
  }

  for (i = 0; i < num_bytes; i++) {
    conv_string[0] = input_string[2*i];
    conv_string[1] = input_string[2*i + 1];
    out[i] = (char) strtol(conv_string, NULL, 16);
  }

  return out;
}

int comp_scores(const void *scorea, const void *scoreb) {
  float a = ((struct keyscore*)scorea)->score;
  float b = ((struct keyscore*)scoreb)->score;

  if (a < b)
    return 1;
  if (a > b)
    return -1;
  return 0;
}

int comp_weighted_hamming(const void *x, const void *y) {
  float a = *((float*)x);
  float b = *((float*)y);

  if (a < b)
    return 1;
  if (a > b)
    return -1;
  return 0;
}

void hex2base64() {
  size_t length = 0;
  ssize_t read;
  ssize_t num_bytes;
  char *input_string = NULL;
  char *bytes = NULL;

  /* Clear stdin */
  scanf("\n");

  /* Quit on empty line */
  while((read = getline(&input_string, &length, stdin)) > 1) {
    /* Remove trailing newline */
    read = read - 1;
    input_string[read] = '\0';
    /* Every 2 hex characters denote 1 byte */
    /* Assume that all input is of even length */
    num_bytes = read/2;

    bytes = hex_to_bytes(input_string, num_bytes);
    print_b64(bytes, num_bytes);
  }

  free(input_string);
  free(bytes);
}
