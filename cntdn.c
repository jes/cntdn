/* cntdn - countdown game simulator

   James Stanley 2010 */

#include "cntdn.h"

/* print the reason on stderr and terminate the program */
void die(const char *reason) {
  fputs(reason, stderr);
  fputc('\n', stderr);
  exit(1);
}

/* return a pointer to a static buffer containing a line read from stdin */
char *get_line(void) {
  static char buf[BUFLEN];

  if(!fgets(buf, BUFLEN, stdin)) die("eof on stdin");

  return buf;
}

int main(int argc, char **argv) {
  srand(time(NULL));

  parse_opts(argc, argv);

  make_players();

  letters_round(); letters_round(); letters_round(); letters_round();
  numbers_round();
  teatime_teaser();

  letters_round(); letters_round(); letters_round(); letters_round();
  origin_of_words();
  numbers_round();
  teatime_teaser();

  letters_round(); letters_round(); letters_round();
  numbers_round();
  conundrum();

  show_scores();

  return 0;
}
