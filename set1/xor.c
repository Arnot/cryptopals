#include "cryptopals.h"
#include <stdlib.h>
#include <stdio.h>

void xor_buffers(char *buffer1, char *buffer2, size_t num_bytes) {
  size_t i;
  char xor_result;

  for (i = 0; i < num_bytes; i++) {
    xor_result = (buffer1[i] ^ buffer2[i]) & 0xFF;
    printf("%02x", xor_result);
  }
  printf("\n");
}
