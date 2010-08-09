/* Player handling for cntdn

   James Stanley 2010 */

#include "cntdn.h"

Player *player;

/* Read and assign name for player n */
static void read_name(int n) {
  char *line;
  char *p;
  int col_num = (n % (num_colours-1)) + 1;

  printf("Name for %splayer %d%s: ", (nocolour ? "" : colour[col_num]), n + 1,
         colour[COLOUR_OFF]);
  line = get_line();

  if((p = strchr(line, '\n'))) *p = '\0';

  if(*line == '\0') player[n].raw_name = strdup(get_name());
  else player[n].raw_name = strdup(line);

  if(nocolour) player[n].name = player[n].raw_name;
  else {
    player[n].name = malloc(strlen(colour[(n + 1) % num_colours]) +
                            strlen(player[n].raw_name) +
                            strlen(colour[COLOUR_OFF]) + 1);
    sprintf(player[n].name, "%s%s%s", colour[(n + 1) % num_colours],
            player[n].raw_name, colour[COLOUR_OFF]);
  }
}

/* compare player scores for qsort */
static int score_cmp(const void *one, const void *two) {
  const int *a = one, *b = two;

  return player[*a].score - player[*b].score;
}

/* Allocate player structures */
void make_players(void) {
  int i;

  player = malloc(sizeof(Player) * players);
  memset(player, 0, sizeof(Player) * players);

  for(i = 0; i < players; i++) read_name(i);
}

/* show player scores, if endgame is non-zero, it will say "player wins with
   N points" for the winning player */
void show_scores(int endgame) {
  int i;
  int *player_order;

  player_order = malloc(sizeof(int) * players);
  for(i = 0; i < players; i++) player_order[i] = i;

  qsort(player_order, players, sizeof(int), score_cmp);

  for(i = 0; i < players; i++) {
    printf("%s %s %d points.\n", player[player_order[i]].name,
           (endgame & (i == players - 1)) ? "wins with" : "has",
           player[player_order[i]].score);
  }
}
