/* Game handling for cntdn

   James Stanley 2010 */

#include "cntdn.h"

static char letter[256];
static int round = 1;

int number[6];
int target;

static sig_atomic_t stop_timer;

/* signal handler to cancel the countdown timer */
static void cancel_timer(int sig) {
  stop_timer = 1;
}

static void run_timer(void) {
  int i;

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
}

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

  for(i = 0; i < num_letters; i++) {
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

/* play one letters round */
void letters_round(void) {
  static int turn = 0;
  int i, j, k;
  int *player_order;

  /* stages:
      pick letters
      30 second timer (let dictionary corner think during this time?)
      reveal word lengths
      reveal words
      assign scores
      dictionary corner words */
  printf(" Round %d: Letters round\n", round);

  printf("It is %s's turn to choose letters.\n", player[turn].name);

  /* read letter choices and generate letters */
  for(i = 0; i < num_letters; i++) get_letter(i);

  /* let people think */
  run_timer();

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

    if(strlen(player[i].word) != player[i].length
       || !valid_word(i, letter)) {
      free(player[i].word);

      /* try again once if they didn't supply a suitable word */
      printf("%s, what is your real %d-letter word? ", player[i].name,
           player[i].length);
      player[i].word = strdup(get_line());

      if(strlen(player[i].word) != player[i].length
         || !valid_word(i, letter)) player[i].length = 0;
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

    if(player[i].length == num_letters) {/* 9 letters score double */
      printf("%d points to %s.\n", num_letters * 2, player[i].name);
      player[i].score += num_letters * 2;
    } else {
      printf("%d points to %s.\n", player[i].length, player[i].name);
      player[i].score += player[i].length;
    }
  }

  /* TODO: display best word in the blue and white style */

  /* ask dictionary corner if they got anything better */
  dict_solve(letter);

  /* TODO: display dictionary corner's word in blue and white */

  /* increment the player whose turn it is to choose letters */
  round++;
  turn++;
  if(turn >= players) turn = 0;

  /* tidy up */
  free(player_order);
  for(i = 0; i < players; i++) free(player[i].word);
}

/* play one numbers round */
void numbers_round(void) {
  int num_large;
  int i, j;
  static int turn = 0;

  /* stages:
      pick numbers
      30 second timer (let Rachel think during this time?)
      reveal answers
      reveal methods
      assign scores
      Rachel's solution */
  printf(" Round %d: Numbers round\n", round);

  printf("It is %s's turn to choose numbers.\n", player[turn].name);

  /* choose numbers */
  init_numbers();
  do {
    printf("%s, how many large numbers? [0 to 4] ", player[turn].name);
    num_large = atoi(get_line());
  } while(num_large < 0 || num_large > 4);

  /* generate numbers */
  for(i = 0; i < num_large; i++) {
    number[i] = get_large();
  }
  for(; i < 6; i++) {
    number[i] = get_small();
  }

  /* display numbers */
  for(j = 6; j >= 0; j--) {
    printf("\r");

    if(nocolour) printf("| ");
    else printf("%s ", letter_colour);

    for(i = 0; i < j; i++) {
      if(number[i] > 9) printf(" ");
      if(number[i] > 99) printf(" ");
      printf("  ");
    }
    for(i = j; i < 6; i++) {
      printf("%d ", number[i]);
    }

    if(nocolour) printf("|");
    else printf("%s", colour_off);
    fflush(stdout);

    usleep(500000);
  }
  putchar('\n');

  speak(RACHEL, "And the target number is...\n");

  /* generate a target */
  for(i = 0; i < 20; i++) {
    printf("  \r");
    if(!nocolour) printf("%s", number_colour);

    printf(" %d ", 100 + (rand() % 900));

    if(!nocolour) printf("%s", colour_off);
    fflush(stdout);

    usleep(100000);
  }
  target = 100 + (rand() % 900);
  printf("  \r");
  if(!nocolour) printf("%s", number_colour);

  printf(" %d ", target);

  if(!nocolour) printf("%s", colour_off);
  printf("\n");

  /* let people think */
  run_timer();

  /* increment the player whose turn it is to choose numbers */
  round++;
  turn++;
  if(turn >= players) turn = 0;
}

/* do a teatime teaser */
void teatime_teaser(void) {
  printf(" *** Teatime teaser\n");
}

/* play the conundrum */
void conundrum(void) {
  printf(" *** Conundrum\n");
}
