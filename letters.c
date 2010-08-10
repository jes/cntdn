/* Letters game solver for cntdn

   This file is used by cntdn as well as by a stand-alone solver.
   Compile normally for a solver, or with CNTDN defined if part of CNTDN

   James Stanley 2010 */

#include "letters.h"

#ifdef CNTDN
#include "cntdn.h"
#endif

TrieNode *dictionary;

#ifndef CNTDN
static int ignore_invalid = 0;
#endif

/* return a number 0..25 to be used as a trie child index, or -1 if it's not a
   usable letter */
static int letter_idx(char c) {
  if(c >= 'A' && c <= 'Z') return c - 'A';
  if(c >= 'a' && c <= 'z') return c - 'a';
  return -1;
}

/* allocate an empty node for the trie, setting all of the fields to zero */
static TrieNode *make_trienode(void) {
  TrieNode *n;

  n = malloc(sizeof(TrieNode));
  memset(n, 0, sizeof(TrieNode));

  return n;
}

/* add the given word to the dictionary trie */
static void add_word(const char *word) {
  static int count_invalid = 0;
  TrieNode *n;
  const char *p;
  int idx;

  n = dictionary;
  p = word;

  while(*p) {
    if((idx = letter_idx(*p)) == -1) {/* invalid letter */
      if(ignore_invalid) return;

      if(count_invalid < 5) {
        fprintf(stderr, "Warning: '%s' contains a non-letter '%c', ignoring "
                        "word; use --ignore-invalid to suppress these "
                        "warnings.\n",
                word, *p);
        count_invalid++;
      } else if(count_invalid == 5) {
        fprintf(stderr, "Warning: more invalid words; suppressing warnings.\n");
        count_invalid = 6;
      }

      return;
    }

    if(!n->child[idx]) n->child[idx] = make_trienode();

    /* go to next node and next character */
    n = n->child[idx];
    p++;
  }

  /* end of the word, mark as such */
  n->end_word = 1;
}

/* Load the dictionary trie from 'path'. set 'maxlen' to the maximum length for
   words to be allowed in the dictionary, or 0 for no limit */
void load_dictionary(const char *path, int maxlen) {
  FILE *fp;
  char *p;
  char buf[BUFLEN];

  if(!(fp = fopen(path, "r")))
    die("Unable to open '%s' for reading. Try using --dictionary", path);

  /* allocate the root node */
  dictionary = make_trienode();

  while(fgets(buf, BUFLEN, fp)) {
    /* accept dos files even on unix; we replace the first occurence of either
       \r or \n with a nul byte */
    if((p = strpbrk(buf, "\r\n"))) *p = '\0';

    /* add the word to the trie */
    if(maxlen == 0 || strlen(buf) <= maxlen) add_word(buf);
  }

  fclose(fp);
}

#ifndef CNTDN
/* What follows from here is only compiled if we're not part of the main cntdn
   program */

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
  "cntdn - countdown game simulator\n"
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

int main(int argc, char **argv) {
  int c;
  char *dict = "./dictionary";
  int minletters = 6;
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
    die("You must specify your letters. See --help for more information.");

  letters = argv[optind];

  load_dictionary(dict, strlen(letters));

  return 0;
}
#endif
