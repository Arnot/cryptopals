#include "cryptopals.h"
#include <stdio.h>
#include <string.h>

unsigned int hamming_distance(char *buf1, char *buf2, size_t num_bytes) {
  unsigned int total = 0;
  unsigned int bytecount;
  unsigned int i;
  unsigned char difference;

  for (i = 0; i < num_bytes; i++) {
    difference = (buf1[i] ^ buf2[i]);

    // Kernighan's method for popcount
    for (bytecount = 0; difference; bytecount++) {
      difference &= difference - 1; // clear the least significant bit set
    }

    total += bytecount;
    /* printf("Bytes: %c(0x%02x), %c(0x%02x). Difference: 0x%02x. popcnt: %d, " */
    /*        "total: %d\n", */
    /*        buf1[i], buf1[i], buf2[i], buf2[i], buf1[i] ^ buf2[i], bytecount, */
    /*        total); */
  }

  return total;
}

#if 0
int main(void) {
  char test[]  = "this is a test";
  char wokka[] = "wokka wokka!!!";

  printf("%u\n", hamming_distance(test, wokka, strlen(test)+1));
}
#endif
