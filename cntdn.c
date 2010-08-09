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
  char *p;

  if(!fgets(buf, BUFLEN, stdin)) die("eof on stdin");

  if((p = strchr(buf, '\n'))) *p = '\0';

  return buf;
}

int main(int argc, char **argv) {
  char *p;

  srand(time(NULL));

  parse_opts(argc, argv);

  make_players();

  for(p = format; *p; p++) {
    switch(tolower(*p)) {
    case 'l': letters_round();   break;
    case 'n': numbers_round();   break;
    case 'g': guest_chat();      break;
    case 't': teatime_teaser();  break;
    case 'o': origin_of_words(); break;
    case 'c': conundrum();       break;
    default:
      fprintf(stderr, "Unknown round letter '%c', skipping round.\n", *p);
    }

    show_scores(*(p+1) == 0);
  }

  return 0;
}
