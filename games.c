/* Game handling for cntdn

   James Stanley 2010 */

#include "cntdn.h"

char letter[10];
int turn;

/* get the nth letter for the current round */
static void get_letter(int n) {
  char *line;
  int repeat;
  int i;

  do {
    repeat = 0;

    printf("%s, vowel or consonant? [vc] ", player[turn].name);
    line = get_line();

    if(tolower(*line) == 'v') letter[n] = get_vowel();
    else if(tolower(*line) == 'c') letter[n] = get_consonant();
    else repeat = 1;
    /* TODO: limitations on amounts of vowels and consonants */
  } while(repeat);

  for(i = 0; i <= n; i++) {
    printf("%c%c", letter[i], (i == n ? '\n' : ' '));
  }
}

/* sort by word length for qsort */
static int length_cmp(const void *one, const void *two) {
  const int *a = one, *b = two;

  return player[*a].length - player[*b].length;
}

/* play one letters round */
void letters_round(void) {
  int i, j;
  int *player_order;

  /* stages:
      pick letters
      30 second timer (let dictionary corner think during this time?)
      reveal word lengths
      reveal words
      assign scores
      dictionary corner words */
  printf(" *** Letters round\n");

  /* read letter choices and generate letters */
  for(i = 0; i < 9; i++) get_letter(i);

  /* wait 30s for players to think */
  for(i = timer; i > 0; i--) {
    printf("\r                ");/* clear the line */
    printf("\r%d seconds.", i);
    fflush(stdout);
    sleep(1);
  }
  printf("\r                \r");

  /* ask each player for their word length */
  for(i = 0; i < players; i++) {
    printf("%s, how long is your word? ", player[i].name);
    player[i].length = atoi(get_line());
  }

  /* get an aray of players sorted by word length */
  player_order = malloc(sizeof(int) * players);
  for(i = 0; i < players; i++) player_order[i] = i;
  qsort(player_order, players, sizeof(int), length_cmp);

  /* ask players for their word, shortest first */
  for(j = 0; j < players; j++) {
    i = player_order[j];

    printf("%s, what is your word? ", player[i].name);
    player[i].word = strdup(get_line());
    /* TODO: check word is correct length */
    /* TODO: ask dictionary corner about it */
  }

  /* increment the player whose turn it is to choose numbers/letters */
  turn++;
  if(turn >= players) turn = 0;

  /* tidy up */
  free(player_order);
  for(i = 0; i < players; i++) free(player[i].word);
}

/* play one numbers round */
void numbers_round(void) {
  printf(" *** Numbers round\n");
}

/* do a teatime teaser */
void teatime_teaser(void) {
  printf(" *** Teatime teaser\n");
}

/* play the conundrum */
void conundrum(void) {
  printf(" *** Conundrum\n");
}
