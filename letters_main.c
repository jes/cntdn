/* Main source file for the letters game solver

   James Stanley 2010 */

#include "cntdn.h"
#include "letters.h"

int ignore_invalid = 0;
int minletters = 6;

static struct option opts[] = {
  { "dictionary",     required_argument, NULL, 'd' },
  { "help",           no_argument,       NULL, 'h' },
  { "ignore-invalid", no_argument,       NULL, 'i' },
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
  "                        ./dictionary)\n"
  "  -h, --help            Display this help\n"
  "  -i, --ignore-invalid  Suppress warnings about invalid words in the\n"
  "                        dictionary\n"
  "  -m, --min-letters=N   Set minimum number of letters (default: 6)\n"
  "\n"
  "For example, if the dictionary is in /usr/share/dict/words and the letters\n"
  "you have are GYHDNOEUR, you could invoke letters like so:\n"
  "  letters --dictionary=/usr/share/dict/words GYHDNOEUR\n"
  "\n"
  "Report bugs to James Stanley <james@incoherency.co.uk>\n"
  );
}

/* callback for solve_letters() to output words of at least minletters */
static void output_words(const char *word) {
  if(strlen(word) >= minletters) puts(word);
}

int main(int argc, char **argv) {
  int c;
  char *dict = "./dictionary";
  char *letters;

  opterr = 1;

  while((c = getopt_long(argc, argv, "d:him:", opts, NULL)) != -1) {
    switch(c) {
      case 'd': dict = optarg;             break;
      case 'h': usage(); exit(0);          break;
      case 'i': ignore_invalid = 1;        break;
      case 'm': minletters = atoi(optarg); break;
      default:  exit(1);                   break;
    }
  }

  if(optind >= argc)
    die("error: you must specify your letters; see --help for more "
        "information");

  letters = argv[optind];

  load_dictionary(dict, strlen(letters));

  solve_letters(letters, output_words);

  return 0;
}
