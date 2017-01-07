#include "cryptopals.h"
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void xor() {
      size_t length1 = 0;
      size_t length2 = 0;
      ssize_t read1, read2;
      ssize_t num_bytes;
      char *input_string1 = NULL;
      char *input_string2 = NULL;
      char *bytes1, *bytes2;
      char *result;

      scanf("\n");

      read1 = getline(&input_string1, &length1, stdin);
      read2 = getline(&input_string2, &length2, stdin);

      if (read1 != read2) {
        printf("Strings are not of equal length\n");
        return;
      }

      num_bytes = read1 / 2;

      bytes1 = hex_to_bytes(input_string1, num_bytes);
      bytes2 = hex_to_bytes(input_string2, num_bytes);

      result = xor_buffers(bytes1, bytes2, num_bytes);

      printf("%s\n", result);

      free(bytes1);
      free(bytes2);
      free(input_string1);
      free(input_string2);
      free(result);
    }

void single_byte_xor() {
  /* size_t length = 0; */
  ssize_t read;
  ssize_t num_bytes;
  char *input =
      "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  char *bytes, *result;
  unsigned char key;
  float score;

  struct keyscore scores[0xFF];

  /* scanf("\n"); */
  /* read = getline(&input, &length, stdin); */
  /* num_bytes = read / 2; */
  printf("%s\n", input);
  read = strlen(input);
  num_bytes = read / 2;

  bytes = hex_to_bytes(input, num_bytes);

  for (key = 0x00; key < 0xFF; key++) {
    result = xor_single_char(bytes, num_bytes, key);
    score = english_similarity_score(result);

    scores[key].key = key; /* needed after sorting */
    scores[key].score = score;

    free(result);
  }

  qsort(scores, 0xFF, sizeof(*scores), comp_scores);

  printf("Top 5 keys with highest similarity to English: \n");
  for (int i = 0; i < 5; i++) {
    result = xor_single_char(bytes, num_bytes, scores[i].key);
    printf("%d.\t key: %x; score = %f\t", i + 1, scores[i].key,
           scores[i].score);
    printf("%s\n", result);

    free(result);
  }

  free(bytes);
}

void find_single_byte_xor() {
  FILE *fp;
  ssize_t read;
  ssize_t num_bytes;
  size_t length = 0;
  char *input_string = NULL;
  char *bytes, *result;
  char *best_match = NULL;
  unsigned char key;
  float score, maxscore;

  size_t line = 0, bestline;

  /* struct keyscore scores[326]; // Lines counted by 'wc -l' */

  fp = fopen("4.txt", "r");

  if (fp == NULL) {
    printf("Challenge 4: Could not open file!\n");
    return;
  }

  maxscore = -1;
  while ((read = getline(&input_string, &length, fp)) > 0 &&
         line < 400) { /* 326 lines in 4.txt */
    num_bytes = read / 2;
    bytes = hex_to_bytes(input_string, num_bytes);

    /* Brute force key to find 'most english' text */
    for (key = 0x00; key < 0xFF; key++) {
      result = xor_single_char(bytes, num_bytes, key);
      score = english_similarity_score(result);

      if (score > maxscore) {
        maxscore = score;
        bestline = line;
        free(best_match);
        best_match = result;
        /* printf("Line %lu - English text found:\n%s\nscore: %f\n", line,
         * best_match, maxscore); */
      } else {
        free(result);
      }
    }

    line++;
    free(bytes);
  }

  printf("Best match: %s\n", best_match);

  free(best_match);
  free(input_string);
  fclose(fp);
}

void repeating_xor() {
  char string[] = "Burning 'em, if you ain't quick and nimble\nI go crazy when "
                  "I hear a cymbal";
  char key[] = "ICE";
  char *result;

  result = repeating_key_xor(string, strlen(string), key, strlen(key));

  printf("Result: \n");
  for (size_t i = 0; i < strlen(string); i++) {
    printf("%02x", (unsigned char)result[i]);
  }
  printf("\n");

  free(result);
}

void break_repeating_xor() {
  unsigned int keysize;
  ssize_t read;
  char *bytes, *input_string = NULL;
  size_t length = 0;
  size_t num_bytes, num_blocks;
  char **blocks;
  char *key, *transposed_buffer;
  FILE *fp;
  char *result_key, *decrypted_text;
  unsigned char testkey, bestkey;
  float score, maxscore;

  fp = fopen("6.hex", "r");
  if (fp == NULL) {
    printf("Challenge 6: Could not open file!\n");
    return;
  }

  read = getline(&input_string, &length, fp);

  num_bytes = strlen(input_string) / 2;

  bytes = hex_to_bytes(input_string, strlen(input_string));

  keysize = find_key_size(bytes, num_bytes);

  blocks = create_blocks(bytes, num_bytes, keysize);
  num_blocks = num_bytes / keysize;

  printf("Keysize: %d, number of blocks: %lu\n", keysize, num_blocks);

  key = malloc(keysize);

  if (key == NULL) {
    printf("break_repeating_xor(): could not allocate memory\n");
    return;
  }

  for (unsigned int i = 0; i < keysize; i++) {
    transposed_buffer = transpose_blocks(blocks, keysize, num_blocks, i);

    maxscore = 0;
    for (testkey = 0x00; testkey < 0xFF; testkey++) {
      result_key = xor_single_char(transposed_buffer, num_blocks, testkey);
      score = english_similarity_score(result_key);

      if (score > maxscore) {
        maxscore = score;
        bestkey = testkey;
      }

      free(result_key);
    }

    decrypted_text = xor_single_char(transposed_buffer, num_blocks, bestkey);

    key[i] = bestkey;
    free(decrypted_text);
    free(transposed_buffer);
  }

  printf("Key:\n%.*s\n\n", (int)keysize, key);

  decrypted_text = repeating_key_xor(bytes, num_bytes, key, keysize);

  printf("Decrypted text:\n\n%.*s\n", (int)num_bytes, decrypted_text);

  cleanup_blocks(blocks, num_blocks);
  free(input_string);
  free(bytes);
}

void decrypt_aes_ecb() {
  unsigned char *key = (unsigned char *)"YELLOW SUBMARINE", *decrypted_text;
  ssize_t read, num_bytes;
  size_t length = 0;
  int decrypt_len;
  char *input_string = NULL, *bytes;
  FILE *fp;
  EVP_CIPHER_CTX *ctx;

  fp = fopen("7.hex", "r");
  if (fp == NULL) {
    printf("Challenge 7: Could not open file!\n");
    return;
  }
  read = getline(&input_string, &length, fp);

  num_bytes = strlen(input_string) / 2;

  bytes = hex_to_bytes(input_string, strlen(input_string));

  decrypted_text = malloc(num_bytes);
  if (decrypted_text == NULL) {
    printf("decrypt_aes_ecb(): error allocating memory\n");
    return;
  }

  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  if (!(ctx = EVP_CIPHER_CTX_new())) {
    printf("Error initializing OPENSSL EVP context\n");
    return;
  }

  EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);

  EVP_DecryptUpdate(ctx, decrypted_text, &decrypt_len, bytes, num_bytes);

  EVP_DecryptFinal_ex(ctx, decrypted_text + decrypt_len, &decrypt_len);

  printf("%s\n", (char *)decrypted_text);

  EVP_CIPHER_CTX_free(ctx);
  EVP_cleanup();
  ERR_free_strings();

  return;
}

void detect_aes_ecb() {
  ssize_t read;
  char *input_string = NULL;
  size_t length = 0;
  int i, j, k;
  FILE *fp;
  ssize_t num_bytes, num_blocks;
  int num_same_blocks, max_same_blocks = 0;
  int block_equal = 0, bestline = 0, line = 1;
  char *bytes, **blocks;

  fp = fopen("8.txt", "r");
  if (fp == NULL) {
    printf("Challenge 7: Could not open file!\n");
    return;
  }

  while ((read = getline(&input_string, &length, fp)) > 0) {
    num_bytes = strlen(input_string) / 2;
    bytes = hex_to_bytes(input_string, num_bytes);
    blocks = create_blocks(bytes, num_bytes, 16);
    num_blocks = num_bytes / 16;
    num_same_blocks = 0;

    for (i = 0; i < num_blocks; i++) {
      for (j = i+1; j < num_blocks; j++) {
        block_equal = 1;

        for (k = 0; k < 16; k++) {
          if (blocks[i][k] != blocks[j][k]) {
            block_equal = 0;
          }
        }

        if (block_equal) {
          num_same_blocks++;
        }
      }
    }

    if (num_same_blocks > max_same_blocks) {
      max_same_blocks = num_same_blocks;
      bestline = line;
    }

    line++;
  }

  printf("Best line: %d. Similar blocks: %d\n", bestline, max_same_blocks);

  free(input_string);
  free(bytes);
  cleanup_blocks(blocks, num_blocks);
}

int main(void) {
  int command = 0;

  printf("Cryptopals Set 1\n");
  do {
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
    printf("1. Hex to base64\n");
    printf("2. Xor of 2 buffers\n");
    printf("3. Brute-force single key xor\n");
    printf("4. Find most-English decrypted sentence\n");
    printf("5. Repeating-key xor\n");
    printf("6. Break repeating-key xor\n");
    printf("7. Decrypt AES in ECB mode\n");
    printf("8. Detect AES in ECB mode\n");
    printf("0. Quit\n");

    if (scanf("%d", &command) == 0) {
      command = 0;
    }

    switch (command) {
    case 1:
      hex2base64();
      break;
    case 2:
      xor();
      break;
    case 3:
      single_byte_xor();
      break;
    case 4:
      find_single_byte_xor();
      break;
    case 5:
      repeating_xor();
      break;
    case 6:
      break_repeating_xor();
      break;
    case 7:
      decrypt_aes_ecb();
    case 8:
      detect_aes_ecb();
      break;
      break;
    default:
      printf("Bye!\n");
      command = 0;
      break;
    }
  } while (command != 0);

  return 0;
}
