/* String lists for cntdn

   TODO: Most of these need to only return a given index once

   James Stanley 2010 */

#include "cntdn.h"

/* random player names */
static const char *name_string[] = {
  "Augustus", "Billy", "Dwayne", "Edmond", "Eric", "Jed", "Joanne", "Susanne",
  "Sylvester", "Wayne", "Zeb"
};
static int num_names = sizeof(name_string) / sizeof(*name_string);

/* letter frequencies from http://www.thecountdownpage.com/letters.htm */
static char vowel_string[] =
  "AAAAAAAAAAAAAAAEEEEEEEEEEEEEEEEEEEEEIIIIIIIIIIIIIOOOOOOOOOOOOOUUUUU";
static int num_vowels = sizeof(vowel_string) / sizeof(*vowel_string);

static char cons_string[] =
  "BBCCCDDDDDDFFGGGHHJKLLLLLMMMMNNNNNNNNPPPPQRRRRRRRRRSSSSSSSSSTTTTTTTTTVWXYZ";
static int num_consonants = sizeof(cons_string) / sizeof(*cons_string);

/* return a random player name */
const char *get_name(void) {
  return name_string[rand() % num_names];
}

/* return a weighted random vowel */
char get_vowel(void) {
  return vowel_string[rand() % num_vowels];
}

/* return a weighted random consonant */
char get_consonant(void) {
  return cons_string[rand() % num_consonants];
}
