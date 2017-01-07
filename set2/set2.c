#include <assert.h>
#include <stdio.h>
#include <string.h>

void pad_pkcs7(char *input, int len_input, char *output, int block_size) {
  int i;
  assert(len_input < block_size);
  unsigned char padding = block_size - len_input;

  for (i = 0; i < len_input; i++) {
    output[i] = input[i];
  }

  for (i = len_input; i < block_size; i++) {
    output[i] = padding;
  }
}

int main() {
  char *input = "YELLOW SUBMARINE";
  char output[1024] = {0};
  unsigned int blocksize = 20;

  pad_pkcs7(input, strlen(input), output, blocksize);

  printf("Padded output: \n");
  for (int i = 0; i < blocksize; i++) {
    printf(" %c ", output[i]);
  }
  printf("\n");
  for (int i = 0; i < blocksize; i++) {
    printf("%2x ", output[i]);
  }
  printf("\n");

  return 0;
}
