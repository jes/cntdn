/* Player handling for cntdn

   James Stanley 2010 */

#include "cntdn.h"

Player *player;

/* Read and assign name for player n */
static void read_name(int n) {
#define BUFLEN 4096
  char buf[BUFLEN];
  char *p;

  printf("Name for player %d: ", n+1);

  if(!fgets(buf, BUFLEN, stdin)) die("eof on stdin");

  if((p = strchr(buf, '\n'))) *p = '\0';

  if(*buf == '\0') player[n].name = strdup(get_name());
  else player[n].name = strdup(buf);
}

/* Allocate player structures */
void make_players(void) {
  int i;

  player = malloc(sizeof(Player) * players);
  memset(player, 0, sizeof(Player) * players);

  for(i = 0; i < players; i++) read_name(i);
}
