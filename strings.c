/* String lists for cntdn

   TODO: Most of these need to only return a given index once

   James Stanley 2010 */

#include "cntdn.h"

/* random player names */
static const char *name_string[] = {
  "Augustus", "Billy", "Dwayne", "Edmond", "Eric", "Jed", "Joanne", "Susanne",
  "Sylvester", "Wayne", "Zeb"
};
static const int num_names = sizeof(name_string) / sizeof(*name_string);

/* letter frequencies from http://www.thecountdownpage.com/letters.htm */
static const char vowel_string[] =
  "AAAAAAAAAAAAAAAEEEEEEEEEEEEEEEEEEEEEIIIIIIIIIIIIIOOOOOOOOOOOOOUUUUU";
static const int num_vowels = sizeof(vowel_string) / sizeof(*vowel_string);

static const char cons_string[] =
  "BBCCCDDDDDDFFGGGHHJKLLLLLMMMMNNNNNNNNPPPPQRRRRRRRRRSSSSSSSSSTTTTTTTTTVWXYZ";
static const int num_consonants = sizeof(cons_string) / sizeof(*cons_string);

/* ANSI colours */
const char const *colour[] = {
  "\e[0m", "\e[31m", "\e[32m", "\e[33m", "\e[34m", "\e[35m", "\e[36m"
};
const int num_colours = sizeof(colour) / sizeof(*colour);

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
