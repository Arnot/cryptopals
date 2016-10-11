#include "cryptopals.h"
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
  free(result);
}

void single_byte_xor() {
  /* size_t length = 0; */
  ssize_t read;
  ssize_t num_bytes;
  char *input = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
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
    printf("%d.\t key: %x; score = %f\t", i+1, scores[i].key, scores[i].score);
    printf("%s\n", result);

    free(result);
  }

  free(bytes);
}

void find_single_byte_xor() {
  FILE* fp;
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
  while((read = getline(&input_string, &length, fp)) && line < 400) { /* 326 lines in 4.txt */
    num_bytes = read/2;
    bytes = hex_to_bytes(input_string, num_bytes);

    /* Brute force key to find 'most english' text */
    for (key = 0x00; key < 0xFF; key++) {
      result = xor_single_char(bytes, num_bytes, key);
      score = english_similarity_score(result);

      if (score > maxscore){
        maxscore = score;
        bestline = line;
        free(best_match);
        best_match = result;
        printf("Line %lu - English text found:\n%s\nscore: %f\n", line, best_match, maxscore);
      } else {
        free(result);
      }
    }

    line++;
    free(bytes);
  }

  printf("Best match: %s\n", best_match);

  free(best_match);
  fclose(fp);
}

int main(void) {
  int command = 0;

  printf("Cryptopals Set 1\n");
  do {
    printf("================\n");
    printf("1. hex to base64\n");
    printf("2. xor of 2 buffers\n");
    printf("3. brute-force single key xor\n");
    printf("4. find most-English decrypted sentence\n");
    printf("0. quit\n");

    scanf("%d", &command);

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
    default:
      printf("Bye!\n");
      command = 0;
      break;
      /* printf("Invalid command\n"); */
    }
  } while (command != 0);

  return 0;
}
