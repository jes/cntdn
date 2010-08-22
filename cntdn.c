/* cntdn - countdown game simulator

   James Stanley 2010 */

#include "cntdn.h"

/* print the reason on stderr and terminate the program */
void die(const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  fputc('\n', stderr);

  exit(1);
}

/* return a pointer to a static buffer containing a line read from stdin */
char *get_line(void) {
  static char buf[BUFLEN];
  char *p;

  if(!noflush) tcflush(STDIN_FILENO, TCIFLUSH);

  if(!fgets(buf, BUFLEN, stdin)) die("error: eof on stdin");

  if((p = strchr(buf, '\n'))) *p = '\0';

  return buf;
}

int main(int argc, char **argv) {
  char *p;

  srand(time(NULL));

  parse_opts(argc, argv);

  load_dictionary(dictionary_path, num_letters);

  make_players();

  for(p = format; *p; p++) {
    switch(tolower(*p)) {
    case 'l': letters_round(); show_scores(*(p+1) == '\0'); break;
    case 'n': numbers_round(); show_scores(*(p+1) == '\0'); break;
    case 'g': guest_chat();                                 break;
    case 't': teatime_teaser();                             break;
    case 'o': origin_of_words();                            break;
    case 'c': conundrum();     show_scores(*(p+1) == '\0'); break;
    default:
      fprintf(stderr, "warning: unknown round letter '%c', skipping round\n",
              *p);
    }
  }

  return 0;
}
