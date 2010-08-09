/* Player handling for cntdn

   James Stanley 2010 */

#include "cntdn.h"

Player *player;

/* Read and assign name for player n */
static void read_name(int n) {
  char *line;
  char *p;
  int col_num = n % num_colours;

  printf("Name for player %d: %s", n + 1, (nocolour ? "" : colour[col_num]));
  line = get_line();
  printf("%s", (nocolour ? "" : colour_off));

  if((p = strchr(line, '\n'))) *p = '\0';

  if(*line == '\0') player[n].raw_name = strdup(get_name());
  else player[n].raw_name = strdup(line);

  /* if we're using colour, add escape sequences to the proper name */
  if(nocolour) player[n].name = player[n].raw_name;
  else {
    player[n].name = malloc(strlen(colour[col_num]) +
                            strlen(player[n].raw_name) +
                            strlen(colour_off) + 1);
    sprintf(player[n].name, "%s%s%s", colour[col_num],
            player[n].raw_name, colour_off);
  }

  /* if a name wasn't supplied, inform the player of his name */
  if(*line == '\0')
    printf("Player %d will be called %s.\n", n + 1, player[n].name);
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
