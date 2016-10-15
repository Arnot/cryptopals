#include "cryptopals.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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

unsigned int find_key_size(char *buffer, size_t num_bytes) {
  unsigned int min_size = 0;
  float min_normalized_dist = 10000;
  unsigned int distance;
  float avg_distance;
  float normalized_dists[MAX_KEYSIZE-MIN_KEYSIZE+1];

  for (unsigned int size = MIN_KEYSIZE; size <= MAX_KEYSIZE && size < num_bytes/3; size++) {
    distance = 0;
    for (unsigned int i = 0; i < num_bytes/size; i++) {
      distance += hamming_distance(&(buffer[i*size]), &(buffer[(i+1)*size]), size);
    }
    avg_distance = (float)(distance)/(float)(num_bytes/size);
    normalized_dists[size - MIN_KEYSIZE] = avg_distance/size;

    if (avg_distance/size < min_normalized_dist) {
      min_normalized_dist = avg_distance/size;
      min_size = size;
      printf("New minimum: %f, %d\n", avg_distance/size, min_size);
    }
  }

  return min_size;
}
