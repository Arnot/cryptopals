#include "cryptopals.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

char **create_blocks(char *input_buffer, size_t num_bytes, size_t blocksize) {
  unsigned int num_blocks = num_bytes/blocksize;
  char **blocks = calloc(num_blocks,sizeof(char*));
  if (blocks == NULL) {
    printf("create_blocks(): Error allocating memory\n");
    return NULL;
  }

  for (unsigned int i = 0; i < num_blocks; i++) {
    blocks[i] = calloc(blocksize,1);

    if (blocks[i] == NULL) {
      printf("create_blocks(): Error allocating memory\n");
      return NULL;
    }
  }

  for (unsigned int i = 0; i < num_blocks; i++) {
    for (unsigned int j = 0; j < blocksize; j++) {
      blocks[i][j] = input_buffer[i*blocksize + j];
    }
  }

  return blocks;
}

void cleanup_blocks(char **blocks, size_t num_blocks) {
  for (unsigned int i = 0; i < num_blocks; i++) {
    free(blocks[i]);
  }

  free(blocks);
}

/* Outputs a vector of all characters in 'position', for each block */
char *transpose_blocks(char **blocks, size_t block_size, size_t num_blocks, size_t position) {
  char *result = calloc(num_blocks,1);

  if (position > block_size-1) {
    printf("transpose_blocks(): invalid position!\n");
  } else {
    for (unsigned int i = 0; i < num_blocks; i++) {
      result[i] = blocks[i][position];
    }
  }

  return result;
}
