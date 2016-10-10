#include "cryptopals.h"
#include <stdio.h>
#include <stdlib.h>

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

void xor() {
  size_t length1 = 0;
  size_t length2 = 0;
  ssize_t read1, read2;
  ssize_t num_bytes;
  char *input_string1 = NULL;
  char *input_string2 = NULL;
  char *bytes1, *bytes2;

  scanf("\n");

  read1 = getline(&input_string1, &length1, stdin);
  read2 = getline(&input_string2, &length2, stdin);

  if (read1 != read2) {
    printf("Strings are not of equal length\n");
    return;
  }

  num_bytes = read1/2;

  bytes1 = hex_to_bytes(input_string1, num_bytes);
  bytes2 = hex_to_bytes(input_string2, num_bytes);

  xor_buffers(bytes1, bytes2, num_bytes);

  free(bytes1);
  free(bytes2);
}

int main(void) {
  int command = 0;

  printf("Cryptopals Set 1\n");
  do {
    printf("================\n");
    printf("1. hex to base64\n");
    printf("2. xor of 2 buffers\n");
    printf("0. quit\n");

    scanf("%d", &command);

    switch(command) {
    case 1:
      hex2base64();
      break;
    case 2:
      xor();
      break;
    case 0:
      printf("Bye!\n");
      break;
    default:
      printf("Invalid command\n");
    }
  } while(command != 0);

  return 0;
}
