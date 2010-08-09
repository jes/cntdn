/* Dictionary corner for cntdn

   James Stanley 2010 */

#include "cntdn.h"

/* Give a Susie Dent monologue on the origin of a word */
void origin_of_words(void) {
  printf(" *** Origin of words\n");
}

/* The guest tells us a story */
void guest_chat(void) {
  printf(" *** Guest tells a story\n");
}

/* Return 1 if word is valid and 0 otherwise. This functions produces game
   output */
int valid_word(const char *word) {
  printf("%sNotSusie%s: \"%s\"! Excellent!\n",
         pres_colour[SUSIE], colour_off, word);

  return 1;
}
