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
static int minletters = 6;
#endif

static int solve_num_letters;
static char *used_letter;

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
        fprintf(stderr, "warning: '%s' contains a non-letter '%c', ignoring "
                        "word; use --ignore-invalid to suppress these "
                        "warnings\n",
                word, *p);
        count_invalid++;
      } else if(count_invalid == 5) {
        fprintf(stderr, "warning: more invalid words; suppressing warnings\n");
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
    die("error: unable to open '%s' for reading; try using --dictionary", path);

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

/* Recursively solve the letters game. Do not call this function, it is used by
   solve_letters() to do the actual solving */
static void recurse_solve(const char *letters, TrieNode *node, char *answer,
                          int level, void (*callback)(const char *word)) {
  int i;
  int idx;
  char done[26] = { 0 };

  if(node->end_word) callback(answer);

  if(level == solve_num_letters) return;

  for(i = 0; i < solve_num_letters; i++) {
    if(used_letter[i]) continue;
    if(done[letter_idx(letters[i])]) continue;

    if((idx = letter_idx(letters[i])) == -1)
      die("error: tried to solve letters '%s' containing non-letter '%c'",
          letters, letters[i]);

    if(node->child[idx]) {
      used_letter[i] = 1;
      answer[level] = letters[i];

      done[letter_idx(letters[i])] = 1;

      recurse_solve(letters, node->child[idx], answer, level+1, callback);

      used_letter[i] = 0;
    }
  }

  answer[level] = '\0';
}

/* Solves the letters game for the given letters by calling 'callback' with
   each of the words found */
void solve_letters(const char *letters, void (*callback)(const char *word)) {
  char *word;

  solve_num_letters = strlen(letters);
  used_letter = malloc(solve_num_letters);
  memset(used_letter, 0, solve_num_letters);

  word = malloc(solve_num_letters + 1);
  memset(word, 0, solve_num_letters + 1);

  recurse_solve(letters, dictionary, word, 0, callback);

  free(word);
  free(used_letter);
}

/* Return 1 if the given word is in the dictionary and 0 otherwise */
int word_in_dictionary(const char *word) {
  TrieNode *n = dictionary;
  const char *p = word;
  int idx;

  while(*p) {
    /* TODO: Should this give a warning? */
    if((idx = letter_idx(*p)) == -1) return 0;

    if(!n->child[idx]) return 0;

    p++;
    n = n->child[idx];

    /* if that was the last letter and the word can end, the word was in the
       dictionary */
    if(!*p && n->end_word) return 1;
  }

  return 0;
}

/* Return 1 if the given word can be made with the given letters */
int can_make_word(const char *word, const char *letters) {
  int avail[26] = { 0 };
  int i, idx;
  const char *p;

  /* add one availability for each letter */
  for(p = letters; *p; p++) {
    if((idx = letter_idx(*p)) == -1) /* TODO: does this need to be fatal? */
      die("error: letters array '%s' contains non-letter '%c'.", letters, *p);

    avail[idx]++;
  }

  /* remove one availability for each letter of the word */
  for(p = word; *p; p++) {
    if((idx = letter_idx(*p)) == -1) return 0;

    avail[idx]--;
  }

  /* if any availabilities are negative, it means more of a letter was used than
     was available */
  for(i = 0; i < 26; i++) {
    if(avail[i] < 0) return 0;
  }

  return 1;
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
#endif
