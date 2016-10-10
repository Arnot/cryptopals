/* #include "cryptopals.h" */
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

/* http://practicalcryptography.com/cryptanalysis/letter-frequencies-various-languages/english-letter-frequencies/
 */
static float english_frequencies[ALPHABET_SIZE] = {
    0.0855, 0.0160, 0.0316, 0.0387, 0.1210, 0.0218, 0.0209, 0.0496, 0.0733,
    0.0022, 0.0081, 0.0421, 0.0253, 0.0717, 0.0747, 0.0207, 0.0010, 0.0633,
    0.0673, 0.0894, 0.0268, 0.0106, 0.0183, 0.0019, 0.0172, 0.0011};

/* static float english_frequencies_squared[ALPHABET_SIZE] = {
       0.00731025, 0.00025600, 0.00099856, 0.00149769, 0.01464100, 0.00047524,
       0.00043681, 0.00246016, 0.00537289, 0.00000484, 0.00006561, 0.00177241,
       0.00064009, 0.00514089, 0.00558009, 0.00042849, 0.00000100, 0.00400689,
       0.00452929, 0.00799236, 0.00071824, 0.00011236, 0.00033489, 0.00000361,
       0.00029584, 0.00000121}; */

/* static float en_sum_squares = 0.06507671; */
static float en_sqrt_sum_squares = 0.25510137;

/* Finds the index of a character in an array */
size_t letterindex(char c) {
  char uppercase = toupper(c);

  return (ssize_t)uppercase - 0x41;
}

float english_similarity(char *text) {
  unsigned int i, alphacount = 0;
  float lettercount[ALPHABET_SIZE] = {0};
  float frequencies[ALPHABET_SIZE] = {0};
  float similarity = 0;
  float sum_mults = 0;
  float sum_squares = 0;
  float sqrt_sum_squares = 0;

  unsigned int length = strlen(text);

  /* Count letters */
  for (i = 0; i < length; i++) {
    if (isalpha(text[i])){
      lettercount[letterindex(text[i])]++;
      alphacount++;
    }
  }

  /* Calculate relative frequency */
  for (i = 0; i < ALPHABET_SIZE; i++) {
    frequencies[i] = (float)lettercount[i] / (float)alphacount;
  }

  /* Calculate cosine similarity */
  for (i = 0; i < ALPHABET_SIZE; i++) {
    sum_mults += frequencies[i] * english_frequencies[i];
    sum_squares += frequencies[i] * frequencies[i];
  }

  sqrt_sum_squares = sqrt(sum_squares);
  similarity = sum_mults / (sqrt_sum_squares * en_sqrt_sum_squares);

  return similarity;
}

/* Testcase */
int main(void) {
  int i;
  int string_pos;

  char nl_test[] =
      "Madame-jeanette is een hete en gele, Surinaamse chilipeper-cultivar, "
      "die vaak wordt verward met de adjuma. Beide variëteiten behoren net als "
      "de Mexicaanse habanero tot de soort Capsicum chinense. De vruchten van "
      "madame-jeanette zijn langer dan de wat meer gedrongen adjuma's, die in "
      "supermarkten vaak als madame-jeanette worden verkocht. De rijpe "
      "vruchten zijn geel en ongeveer even scherp als de oranje habanero";
  char en_test[] =
      "Cosine similarity is a measure of similarity between two non zero "
      "vectors of an inner product space that measures the cosine of the angle "
      "between them. The cosine of 0° is 1, and it is less than 1 for any "
      "other angle. It is thus a judgment of orientation and not magnitude: "
      "two vectors with the same orientation have a cosine similarity of 1, "
      "two vectors at 90° have a similarity of 0, and two vectors "
      "diametrically opposed have a similarity of -1, independent of their "
      "magnitude. Cosine similarity is particularly used in positive space, "
      "where the outcome is neatly bounded in [0,1].";

  printf("NL: %f\nEN: %f\n", english_similarity(nl_test),
         english_similarity(en_test));

  return 0;
}
