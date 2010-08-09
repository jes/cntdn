/* cntdn - countdown game simulator

   James Stanley 2010 */

#include "cntdn.h"

/* print the reason on stderr and terminate the program */
void die(const char *reason) {
  fputs(reason, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char **argv) {
  int i;

  srand(time(NULL));

  parse_opts(argc, argv);

  make_players();

  for(i = 0; i < players; i++)
    printf("Player %d is %s.\n", i+1, player[i].name);

  return 0;
}
