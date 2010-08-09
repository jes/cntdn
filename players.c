/* Player handling for cntdn

   James Stanley 2010 */

#include "cntdn.h"

Player *player;

/* Read and assign name for player n */
static void read_name(int n) {
  char *line;
  char *p;

  printf("Name for player %d: ", n+1);
  line = get_line();

  if((p = strchr(line, '\n'))) *p = '\0';

  if(*line == '\0') player[n].name = strdup(get_name());
  else player[n].name = strdup(line);
}

/* Allocate player structures */
void make_players(void) {
  int i;

  player = malloc(sizeof(Player) * players);
  memset(player, 0, sizeof(Player) * players);

  for(i = 0; i < players; i++) read_name(i);
}

/* show player scores */
void show_scores(void) {
  int i;

  for(i = 0; i < players; i++) {
    printf("%s has %d points.\n", player[i].name, player[i].score);
  }
}
