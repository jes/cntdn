/* Dictionary corner for cntdn

   James Stanley 2010 */

#include "cntdn.h"

static char best_word[10];

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
  /* TODO: this function */
  printf("%sNotSusie%s: \"%s\"! Excellent!\n",
         pres_colour[SUSIE], colour_off, word);

  /* NOTE: if the word isn't in the dictionary, the user should be asked if
     they want to use it anyway */

  return 1;
}

/* callback for solve_letters to store best word */
static void store_word(const char *word) {
  if(strlen(word) > strlen(best_word)) strcpy(best_word, word);
}

/* Get dictionary corner to give their best word */
void dict_solve(const char *letters) {
  /* TODO: this function should be more intelligent about when it gets a word
     that one of the players got */

  best_word[0] = '\0';

  solve_letters(letters, store_word);

  printf("%sNotSusie%s: The best word we found was \"%s\".\n",
         pres_colour[SUSIE], colour_off, best_word);
}
