/* Dictionary corner for cntdn

   James Stanley 2010 */

#include "cntdn.h"

static char best_word[256];

/* Give a Susie Dent monologue on the origin of a word */
void origin_of_words(void) {
  printf(" *** Origin of words\n");
}

/* The guest tells us a story */
void guest_chat(void) {
  printf(" *** Guest tells a story\n");
}

/* Return 1 if player p's word is valid and 0 otherwise. This functions produces
   game output */
int valid_word(int p, const char *letters) {
  char *line;

  if(!can_make_word(player[p].word, letters)) {
    /* TODO: this should be more specific about what letters are missing */
    printf("%sNotSusie%s: Sorry %s, you can't make \"%s\" with those "
           "letters.\n", pres_colour[SUSIE], colour_off, player[p].name,
           player[p].word);
    return 0;
  }

  if(!word_in_dictionary(player[p].word)) {
    while(1) {
      printf("%sNotSusie%s: %s, \"%s\" isn't in my dictionary. Do you want to "
             "use it anyway? [yn] ", pres_colour[SUSIE], colour_off,
             player[p].name, player[p].word);

      line = get_line();

      if(tolower(*line) == 'y') return 1;
      else if(tolower(*line) == 'n') return 0;
    }
  }

  printf("%sNotSusie%s: \"%s\"! Excellent!\n", pres_colour[SUSIE], colour_off,
         player[p].word);

  return 1;
}

/* callback for solve_letters to store best word */
static void store_word(const char *word) {
  if(strlen(word) > strlen(best_word)) strcpy(best_word, word);
}

/* Get dictionary corner to give their best word */
void dict_solve(const char *letters) {
  int i;

  /* TODO: this function should be more intelligent about when it gets a word
     that one of the players got */

  best_word[0] = '\0';

  solve_letters(letters, store_word);

  for(i = 0; best_word[i]; i++) {
    best_word[i] = tolower(best_word[i]);
  }

  if(!*best_word) {
    printf("%sNotSusie%s: We couldn't find any words at all.\n",
           pres_colour[SUSIE], colour_off);
  } else {
    printf("%sNotSusie%s: The best word we found was \"%s\".\n",
           pres_colour[SUSIE], colour_off, best_word);
  }
}
