/* Player handling for cntdn

   James Stanley 2010 */

#include "cntdn.h"

Player *player;

/* Read and assign name for player n */
static void read_name(int n) {
  char *line;
  int col_num = n % num_colours;
  size_t len;

  if(players == 1) printf("Name for player: %s", colour[col_num]);
  else printf("Name for player %d: %s", n + 1, colour[col_num]);

  line = get_line();
  printf("%s", colour_off);

  if(*line == '\0') player[n].raw_name = strdup(get_name());
  else player[n].raw_name = strdup(line);

  /* add colour to the printing name */
  len = strlen(colour[col_num]) + strlen(player[n].raw_name)
        + strlen(colour_off) + 1;
  player[n].name = malloc(len);
  snprintf(player[n].name, len, "%s%s%s", colour[col_num],
          player[n].raw_name, colour_off);

  /* if a name wasn't supplied, inform the player of his name */
  if(*line == '\0')
    printf("Player %d will be called %s.\n", n + 1, player[n].name);
}

/* compare player scores for qsort */
static int score_cmp(const void *one, const void *two) {
  const int *a = one, *b = two;

  return player[*b].score - player[*a].score;
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

  printf("\n");

  for(i = 0; i < players; i++) {
    printf("  %s %s %d points.\n", player[player_order[i]].name,
           (endgame &
             (player[player_order[i]].score == player[player_order[0]].score) ?
               (players == 0 ? "finishes with" : "wins with") : "has"),
           player[player_order[i]].score);
  }

  printf("\n");
}
