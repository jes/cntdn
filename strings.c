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
static const int num_vowels =
  sizeof(vowel_string) / sizeof(*vowel_string) - 1;

static const char cons_string[] =
  "BBCCCDDDDDDFFGGGHHJKLLLLLMMMMNNNNNNNNPPPPQRRRRRRRRRSSSSSSSSSTTTTTTTTTVWXYZ";
static const int num_consonants =
  sizeof(cons_string) / sizeof(*cons_string) - 1;

/* a shuffled array of vowels. this is re-initialised from vowel_string whenever
   it becomes empty, and then letters are subsequently read from vowel[0],
   vowel[1], vowel[2], etc. */
static char vowel[sizeof(vowel_string) / sizeof(*vowel_string)];
static int vowel_pos = sizeof(vowel) / sizeof(*vowel);

/* as for vowel[] above, but for consonants */
static char consonant[sizeof(cons_string) / sizeof(*cons_string)];
static int cons_pos = sizeof(consonant) / sizeof(*consonant);

/* ANSI colours */
char *colour_off = "\e[0m";
char *letter_colour = "\e[1;37;44m";

/* player colours */
char *colour[] = {
  "\e[31m", "\e[32m", "\e[33m", "\e[34m", "\e[35m", "\e[36m"
};
const int num_colours = sizeof(colour) / sizeof(*colour);

/* presenter colours */
char *pres_colour[] = {
  /* Jeff   */ "\e[1;31m", /* Susie */ "\e[1;32m",
  /* Rachel */ "\e[1;35m", /* Guest */ "\e[1;34m"
};

/* shuffles character array 'a' of length 'len' */
static void shuffle(char *a, int len) {
  int i, j;
  char c;

  for(i = len-1; i >= 0; i--) {
    j = rand() % (i + 1);
    c = a[i];
    a[i] = a[j];
    a[j] = c;
  }
}

/* initialise the shuffled vowel array */
static void init_vowels(void) {
  strcpy(vowel, vowel_string);
  shuffle(vowel, num_vowels);
  vowel_pos = 0;
}

/* initialise the shuffled consonant array */
static void init_consonants(void) {
  strcpy(consonant, cons_string);
  shuffle(consonant, num_consonants);
  cons_pos = 0;
}

/* return a random player name */
const char *get_name(void) {
  return name_string[rand() % num_names];
}

/* return a weighted random vowel */
char get_vowel(void) {
  if(vowel_pos >= num_vowels) init_vowels();
  return vowel[vowel_pos++];
}

/* return a weighted random consonant */
char get_consonant(void) {
  if(cons_pos >= num_consonants) init_consonants();
  return consonant[cons_pos++];
}
