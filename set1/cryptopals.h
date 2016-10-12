#ifndef _CRYPTOPALS_H_
#define _CRYPTOPALS_H_
#include <stdlib.h>

struct keyscore {
  unsigned char key;
  float score;
};

void print_b64(char *bytes, size_t length);
char *hex_to_bytes(char *input_string, unsigned int length);
void hex2base64();
int comp_scores(const void *scorea, const void *scoreb);
char *xor_buffers(char *buffer1, char *buffer2, size_t num_bytes);
char *xor_single_char(char *buffer, size_t num_bytes, char key);
char *repeating_key_xor(char* buffer, size_t num_bytes, char* key, size_t key_size);

float english_similarity_score(char *text);

#endif
