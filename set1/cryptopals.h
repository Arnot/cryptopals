#ifndef _CRYPTOPALS_H_
#define _CRYPTOPALS_H_
#include <stdlib.h>

void print_b64(char *bytes, size_t length);
char *hex_to_bytes(char *input_string, unsigned int length);
void xor_buffers(char *buffer1, char *buffer2, size_t num_bytes);

#endif
