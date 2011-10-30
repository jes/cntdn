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
    speak(SUSIE, "Sorry %s, you can't make \"%s\" with those letters.",
          player[p].name, player[p].word);

    if(!word_in_dictionary(player[p].word))
      slow_printf(" It isn't in my dictionary, either.");

    printf("\n");
    return 0;
  }

  if(!word_in_dictionary(player[p].word)) {
    while(1) {
      speak(SUSIE, "%s, \"%s\" isn't in my dictionary. Do you want to use it "
            "anyway? [yn] ", player[p].name, player[p].word);

      line = get_line();

      if(tolower(*line) == 'y') return 1;
      else if(tolower(*line) == 'n') return 0;
    }
  }

  /* TODO: decide whether the word is good or not and react accordingly */
  speak(SUSIE, "\"%s\"! Excellent!\n", player[p].word);

  return 1;
}

/* callback for solve_letters to store best word */
static void store_word(const char *word, void *unused) {
  if(strlen(word) > strlen(best_word)) strncpy(best_word, word, 255);
  best_word[255] = '\0';
}

/* Get dictionary corner to give their best word */
void dict_solve(const char *letters) {
  int i;

  /* TODO: this function should be more intelligent about when it gets a word
     that one of the players got */

  best_word[0] = '\0';

  solve_letters(letters, store_word, NULL);

  for(i = 0; best_word[i]; i++) {
    best_word[i] = tolower(best_word[i]);
  }

  if(!*best_word) {
    speak(SUSIE, "We couldn't find any words at all.\n");
  } else {
    speak(SUSIE, "The best word we found was \"%s\".\n", best_word);
  }
}
