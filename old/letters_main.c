/* Main source file for the letters game solver

   James Stanley 2010 */

#include "letters.h"

int ignore_invalid = 0;
int interactive_mode = 0;
int minletters = 6;

static struct option opts[] = {
  { "dictionary",     required_argument, NULL, 'd' },
  { "help",           no_argument,       NULL, 'h' },
  { "ignore-invalid", no_argument,       NULL, 'i' },
  { "interactive",    no_argument,       NULL, 'I' },
  { "min-letters",    required_argument, NULL, 'm' },
  { NULL,             0,                 NULL, 0 }
};

/* print the reason on stderr and terminate the program */
void die(const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  fputc('\n', stderr);

  exit(1);
}

static void usage(void) {
  printf(
  "letters - countdown letters game solver\n"
  "Usage: letters [OPTIONS...] LETTERS\n"
  "\n"
  "Options:\n"
  "  -d, --dictionary=STR  Set the path to the dictionary file (default:\n"
  "                        " DEFAULT_DICT ")\n"
  "  -h, --help            Display this help\n"
  "  -i, --ignore-invalid  Suppress warnings about invalid words in the\n"
  "                        dictionary\n"
  "  -I, --interactive     Use interactive mode; see below for details\n"
  "  -m, --min-letters=N   Set minimum number of letters (default: 6)\n"
  "\n"
  "For example, if the dictionary is in /usr/share/dict/words and the letters\n"
  "you have are GYHDNOEUR, you could invoke letters like so:\n"
  "  letters --dictionary=/usr/share/dict/words GYHDNOEUR\n"
  "\n"
  "Interactive mode should be used when performance is important and you have\n"
  "more than one set of letters that need solving. Give a stream of letter\n"
  "sets, one per line, on stdin. This program will give all of the solutions\n"
  "for the first set, followed by a blank line, followed by the next set,\n"
  "etc.\n"
  "\n"
  "Report bugs to James Stanley <james@incoherency.co.uk>\n"
  );
}

/* callback for solve_letters() to output words of at least minletters */
static void output_words(const char *word, void *unused) {
  if(strlen(word) >= minletters) puts(word);
}

int main(int argc, char **argv) {
  int c;
  char *dict = DEFAULT_DICT;
  char *letters;
  char buf[512];
  char *p;

  opterr = 1;

  while((c = getopt_long(argc, argv, "d:hiIm:", opts, NULL)) != -1) {
    switch(c) {
      case 'd': dict = optarg;             break;
      case 'h': usage(); exit(0);          break;
      case 'i': ignore_invalid = 1;        break;
      case 'I': interactive_mode = 1;      break;
      case 'm': minletters = atoi(optarg); break;
      default:  exit(1);                   break;
    }
  }

  if(interactive_mode) {
    if(optind < argc)
      die("error: you must specify your letters on stdin when using "
          "interactive mode; see --help for more information");

    load_dictionary(dict, 0);

    /* disable block buffering */
    setvbuf(stdout, NULL, _IOLBF, 0);

    /* read letters from stdin until EOF */
    while(fgets(buf, 512, stdin)) {
      if((p = strchr(buf, '\n'))) *p = '\0';

      solve_letters(buf, output_words, NULL);

      putchar('\n');
      fflush(stdout);
    }
  } else /* not interactive_mode */ {
    if(optind >= argc)
      die("error: you must specify your letters; see --help for more "
          "information");

    letters = argv[optind];

    load_dictionary(dict, strlen(letters));

    solve_letters(letters, output_words, NULL);
  }

  return 0;
}
