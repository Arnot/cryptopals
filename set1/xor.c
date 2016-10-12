#include "cryptopals.h"
#include <stdio.h>
#include <stdlib.h>

char *xor_buffers(char *buffer1, char *buffer2, size_t num_bytes) {
  size_t i;
  char *out = malloc(num_bytes);

  if (out == NULL) {
    printf("xor_buffers: Error allocating memory\n");
    return NULL;
  }

  for (i = 0; i < num_bytes; i++) {
    out[i] = (buffer1[i] ^ buffer2[i]) & 0xFF;
  }
  return out;
}

char *xor_single_char(char *buffer, size_t num_bytes, char key) {
  size_t i;
  char *out = calloc(num_bytes,num_bytes);

  if (out == NULL) {
    printf("xor_single_char: Error allocating memory\n");
    return NULL;
  }

  for (i = 0; i < num_bytes; i++) {
    out[i] = (buffer[i] ^ key) & 0xFF;
  }

  return out;
}

char *repeating_key_xor(char* buffer, size_t num_bytes, char* key, size_t key_size) {
  size_t i;
  char *out = calloc(num_bytes, num_bytes);

  if (out == NULL) {
    printf("repeating_key_xor: Error allocating memory\n");
    return NULL;
  }

  for (i = 0; i < num_bytes; i++) {
    out[i] = (buffer[i] ^ key[i%key_size]) & 0xFF;
  }

  return out;
}
