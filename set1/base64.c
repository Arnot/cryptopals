#include "cryptopals.h"
#include <stdio.h>
#include <stdlib.h>

char *base64chars =
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

/* Converts 1-3 bytes to base64 */
void convert_b64(char *bytes, unsigned int len) {
  unsigned char c1 = 0, c2 = 0, c3 = 0, c4 = 0;

  if (len > 3) {
    printf("Invalid length for base64 conversion!\n");
  } else {
    c1 = (bytes[0] & 0xFC) >> 2;

    if (len == 1) { // Pad with '=='
      c2 = ((bytes[0] & 0x03) << 4);
      c3 = 64;
      c4 = 64;
    } else if (len == 2) { // Pad with 1 '='
      c2 = ((bytes[0] & 0x03) << 4) | ((bytes[1] & 0xF0) >> 4);
      c3 = (bytes[1] & 0x0F) << 2;
      c4 = 64;
    } else {
      c2 = ((bytes[0] & 0x03) << 4) | ((bytes[1] & 0xF0) >> 4);
      c3 = ((bytes[1] & 0x0F) << 2) | ((bytes[2] & 0xC0) >> 6);
      c4 = (bytes[2] & 0x3F);
    }
  }

  printf("%c%c%c%c", base64chars[c1], base64chars[c2], base64chars[c3],
         base64chars[c4]);

  /* printf(", %d %d %d %d", c1, c2, c3, c4); */
}

void print_b64(char *bytes, size_t length) {
  int processed = 0;
  int remaining;
  char block[3];

  remaining = length - processed;
  while (remaining > 0) {
    if (remaining >= 3) {
      block[0] = bytes[processed];
      block[1] = bytes[processed+1];
      block[2] = bytes[processed+2];

      processed += 3;

      convert_b64(block, 3);
    } else {
      convert_b64(&bytes[processed], remaining);
      processed += remaining;
    }

    remaining = length - processed;
  }
  printf("\n");
}

