/* Letters game solver for cntdn

   This file is used by cntdn as well as by a stand-alone solver.
   Compile normally for a solver, or with CNTDN defined if part of CNTDN

   James Stanley 2010 */

#include "letters.h"

#ifdef CNTDN
#include "cntdn.h"
#endif

TrieNode *dictionary;

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
  TrieNode *n;
  const char *p;
  int idx;

  n = dictionary;
  p = word;

  while(*p) {
    if((idx = letter_idx(*p)) == -1) {
      fprintf(stderr, "Warning: '%s' contains a non-letter '%c', ignoring word."
                      " Use --ignore-invalid to suppress these warnings.\n",
              word, *p);
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

/* Load the dictionary trie from 'path' */
void load_dictionary(const char *path) {
  FILE *fp;
  char *p;
  char buf[BUFLEN];

  if(!(fp = fopen(path, "r")))
    die("Unable to open '%s' for reading. Try using --dictionary\n", path);

  /* allocate the root node */
  dictionary = make_trienode();

  while(fgets(buf, BUFLEN, fp)) {
    /* accept dos files even on unix; we replace the first occurence of either
       \r or \n with a nul byte */
    if((p = strpbrk(buf, "\r\n"))) *p = '\0';

    /* add the word to the trie */
    add_word(buf);
  }

  fclose(fp);
}

#ifndef CNTDN
/* What follows from here is only compiled if we're not part of the main cntdn
   program */

static struct option opts[] = {
  { "dictionary", required_argument, NULL, 'd' },
  { "help",       no_argument,       NULL, 'h' },
  { NULL,         0,                 NULL, 0 }
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
  "\n"
  "For example, if the dictionary is in /usr/share/dict/words and the letters\n"
  "you have are GYHDNOEUR, you would invoke letters like so:\n"
  "  letters --dictionary=/usr/share/dict/words GYHDNOEUR\n"
  "\n"
  "Report bugs to James Stanley <james@incoherency.co.uk>\n"
  );
}

int main(int argc, char **argv) {
  int c;
  char *dict;

  opterr = 1;

  while((c = getopt_long(argc, argv, "d:h", opts, NULL)) != -1) {
    switch(c) {
      case 'd': dict = optarg;    break;
      case 'h': usage(); exit(0); break;
      default:  exit(1);          break;
    }
  }

  load_dictionary(dict);

  return 0;
}
#endif
