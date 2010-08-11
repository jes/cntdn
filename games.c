/* Game handling for cntdn

   James Stanley 2010 */

#include "cntdn.h"

static char letter[10];
static int turn;

static sig_atomic_t stop_timer;

/* get the nth letter for the current round */
static void get_letter(int n) {
  char *line;
  int i;

  while(1) {
    printf("vowel or consonant? [vc] ");
    line = get_line();

    if(tolower(*line) == 'v') {
      letter[n] = get_vowel();
      break;
    } else if(tolower(*line) == 'c') {
      letter[n] = get_consonant();
      break;
    }

    /* TODO: limitations on amounts of vowels and consonants */
  }

  if(nocolour) printf("| ");
  else printf("%s ", letter_colour);

  for(i = 0; i < 9; i++) {
    printf("%c ", (i <= n ? letter[i] : ' '));
  }

  if(nocolour) printf("|\n");
  else printf("%s\n", colour_off);
}

/* sort by word length for qsort */
static int length_cmp(const void *one, const void *two) {
  const int *a = one, *b = two;

  return player[*a].length - player[*b].length;
}

/* signal handler to cancel the countdown timer */
static void cancel_timer(int sig) {
  stop_timer = 1;
}

/* play one letters round */
void letters_round(void) {
  int i, j, k;
  int *player_order;

  /* stages:
      pick letters
      30 second timer (let dictionary corner think during this time?)
      reveal word lengths
      reveal words
      assign scores
      dictionary corner words */
  printf(" *** Letters round\n");

  printf("It is %s's turn to choose letters.\n", player[turn].name);

  /* read letter choices and generate letters */
  for(i = 0; i < 9; i++) get_letter(i);

  /* install signal handler that will cancel the timer */
  stop_timer = 0;
  signal(SIGINT, cancel_timer);

  /* wait for players to think */
  for(i = timer; i >= 0 && !stop_timer; i--) {
    printf("\r                     ");/* clear the line */
    printf("\r%d second%s left.", i, (i == 1 ? "" : "s"));
    fflush(stdout);
    if(i > 0) sleep(1);
  }
  printf("\n");

  /* re-instate default SIGINT handler */
  signal(SIGINT, SIG_DFL);

  /* ask each player for their word length */
  for(i = 0; i < players; i++) {
    printf("%s, how long is your word? ", player[(i + turn) % players].name);
    player[(i + turn) % players].length = atoi(get_line());
  }

  /* get an aray of players sorted by word length */
  player_order = malloc(sizeof(int) * players);
  for(i = 0; i < players; i++) player_order[i] = i;
  qsort(player_order, players, sizeof(int), length_cmp);

  /* ask players for their word, shortest first */
  for(j = 0; j < players; j++) {
    i = player_order[j];

    if(player[i].length <= 0) {
      player[i].word = NULL;
      continue;
    }

    printf("%s, what is your %d-letter word? ", player[i].name,
           player[i].length);
    player[i].word = strdup(get_line());

    if(strlen(player[i].word) != player[i].length) {
      free(player[i].word);

      /* try again once if they didn't supply a suitable word */
      printf("%s, what is your real %d-letter word? ", player[i].name,
           player[i].length);
      player[i].word = strdup(get_line());

      if(strlen(player[i].word) != player[i].length) player[i].length = 0;
    }

    if(player[i].length > 0) {
      /* check the word with dictionary corner */
      if(!valid_word(i, letter)) player[i].length = 0;
    }
  }

  /* re-sort to get non-words removed */
  qsort(player_order, players, sizeof(int), length_cmp);

  /* find the best scorers */
  for(j = players - 2; j >= 0; j--) {
    i = player_order[j];

    if(player[i].length < player[player_order[players - 1]].length) break;
  }

  /* assign points to the scorers */
  for(k = players - 1; k > j; k--) {
    i = player_order[k];

    if(player[i].length == 9) {/* 9 letters score double */
      printf("18 points to %s.\n", player[i].name);
      player[i].score += 18;
    } else {
      printf("%d points to %s.\n", player[i].length, player[i].name);
      player[i].score += player[i].length;
    }
  }

  /* TODO: display best word in the blue and white style */

  /* ask dictionary corner if they got anything better */
  dict_solve(letter);

  /* TODO: display dictionary corner's word in blue and white */

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
