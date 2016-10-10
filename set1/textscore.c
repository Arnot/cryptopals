#include "cryptopals.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26 + 1 /* Spaces are also counted */

/* See table with calculations (org-mode) below */
static float english_frequencies[ALPHABET_SIZE] = {
    0.0651738, 0.0124248, 0.0217339, 0.0349835, 0.1041442, 0.0197881, 0.0158610,
    0.0492888, 0.0558094, 0.0009033, 0.0050529, 0.0331490, 0.0202124, 0.0564513,
    0.0596302, 0.0137645, 0.0008606, 0.0497563, 0.0515760, 0.0729357, 0.0225134,
    0.0082903, 0.0171272, 0.0013692, 0.0145984, 0.0007836, 0.1918182};

/* static float en_sum_squares = 0.079932031; */
static float en_sqrt_sum_squares = 0.28272253;

/* Finds the index of a character in the frequency- array */
size_t letterindex(char c) {
  char uppercase = toupper(c);

  return (ssize_t)uppercase - 0x41;
}

float english_similarity_score(char *text) {
  unsigned int i, alphacount = 0, spacecount = 0;
  float lettercount[ALPHABET_SIZE] = {0};
  float frequencies[ALPHABET_SIZE] = {0};
  float similarity = 0;
  float sum_mults = 0;
  float sum_squares = 0;
  float sqrt_sum_squares = 0;

  unsigned int length = strlen(text);

  /* Count letters */
  for (i = 0; i < length; i++) {
    if (isalpha(text[i])) {
      lettercount[letterindex(text[i])]++;
      alphacount++;
    }
    if (text[i] == ' ') {
      lettercount[ALPHABET_SIZE-1]++;
      spacecount++;
    }
  }

  // No letters == not usable
  if (alphacount == 0) {
    return 0;
  }

  /* Calculate relative frequency */
  for (i = 0; i < ALPHABET_SIZE; i++) {
    frequencies[i] = (float)lettercount[i] / (float)(alphacount+spacecount);
  }

  /* Calculate cosine similarity */
  for (i = 0; i < ALPHABET_SIZE; i++) {
    sum_mults += frequencies[i] * english_frequencies[i];
    sum_squares += frequencies[i] * frequencies[i];
  }

  sqrt_sum_squares = sqrt(sum_squares);
  similarity = sum_mults / (sqrt_sum_squares * en_sqrt_sum_squares);

  return similarity;// * alphacount;
}

/* Testcase */
#if 0
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

  printf("NL: %f\nEN: %f\n",
         english_similarity_score(nl_test),
         english_similarity_score(en_test));

  return 0;
}
#endif

/*
|     | relative freq |       freq^2 |           |
|-----+---------------+--------------+-----------|
| a   |     0.0651738 | 4.2476242e-3 |           |
| b   |     0.0124248 | 1.5437566e-4 |           |
| c   |     0.0217339 | 4.7236241e-4 |           |
| d   |     0.0349835 | 1.2238453e-3 |           |
| e   |     0.1041442 |  0.010846014 |           |
| f   |     0.0197881 | 3.9156890e-4 |           |
| g   |     0.0158610 | 2.5157132e-4 |           |
| h   |     0.0492888 | 2.4293858e-3 |           |
| i   |     0.0558094 | 3.1146891e-3 |           |
| j   |     0.0009033 | 8.1595089e-7 |           |
| k   |     0.0050529 | 2.5531798e-5 |           |
| l   |     0.0331490 | 1.0988562e-3 |           |
| m   |     0.0202124 | 4.0854111e-4 |           |
| n   |     0.0564513 | 3.1867493e-3 |           |
| o   |     0.0596302 | 3.5557608e-3 |           |
| p   |     0.0137645 | 1.8946146e-4 |           |
| q   |     0.0008606 | 7.4063236e-7 |           |
| r   |     0.0497563 | 2.4756894e-3 |           |
| s   |     0.0515760 | 2.6600838e-3 |           |
| t   |     0.0729357 | 5.3196163e-3 |           |
| u   |     0.0225134 | 5.0685318e-4 |           |
| v   |     0.0082903 | 6.8729074e-5 |           |
| w   |     0.0171272 | 2.9334098e-4 |           |
| x   |     0.0013692 | 1.8747086e-6 |           |
| y   |     0.0145984 | 2.1311328e-4 |           |
| z   |     0.0007836 | 6.1402896e-7 |           |
| spc |     0.1918182 |  0.036794222 |           |
|-----+---------------+--------------+-----------|
|     |            1. |  0.079932031 | sum       |
|     |               |   0.28272253 | sqrt(sum) |
#+TBLFM: @31$3=sqrt(@-1)::@30$2=vsum(@2..@28)::@30$3=vsum(@2..@28)

Numbers from http://www.data-compression.com/english.html
*/
