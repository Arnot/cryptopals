#include "cryptopals.h"
#include <stdlib.h>
#include <stdio.h>

char *hex_to_bytes(char *input_string, unsigned int num_bytes) {
  int i;

  char *out = malloc(num_bytes);
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
