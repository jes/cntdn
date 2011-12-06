/* Letters game solver for cntdn

   This file is used by cntdn and lettersd as well as by the stand-alone solver

   James Stanley 2011 */

#include "letters.h"

TrieNode *dictionary;

/* this table is filled in by load_dictionary(). Before this table was
   introduced, 40% of execution time in solving letters (excluding loading the
   dictionary) was spent calculating letter indices. The table reduces that to
   about 1% */
static int idx_table[256];
#define letter_idx(c) (idx_table[(unsigned int)(c & 0xff)])

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
  int i;

  /* initialise the letter_idx table */
  for(i = 0; i < 256; i++) {
    if(i >= 'A' && i <= 'Z') idx_table[i] = i - 'A';
    else if(i >= 'a' && i <= 'z') idx_table[i] = i - 'a';
    else idx_table[i] = -1;
  }

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
                          int level, int nletters, char *used_letter,
                          void (*callback)(const char *word, void *data),
                          void *data) {
  int i;
  int idx;
  char done[26] = { 0 };

  /* if this node can represent the end of a word, output it */
  if(node->end_word) callback(answer, data);

  /* if all child nodes represent words which are too long, stop searching */
  if(level == nletters) return;

  /* for each of the letters we have */
  for(i = 0; i < nletters; i++) {
    /* if this letter is already used do not use it again */
    if(used_letter[i]) continue;

    /* if this is not an actual letter do not try to use it */
    if((idx = letter_idx(letters[i])) == -1) continue;

    /* if we have already recursed on this letter index, don't bother trying
     * again - this prevents recursing on 'a' twice and getting duplicated
     * output.
     */
    if(done[idx]) continue;

    /* if there is a child with this letter, recurse.
     * if the current node represents 'fo' and the current letter is 'o', the
     * child node represents 'foo'.
     */
    if(node->child[idx]) {
      /* don't allow this letter to be used twice */
      used_letter[i] = 1;

      /* append to the answer */
      answer[level] = letters[i];

      /* don't recurse on the same character again */
      done[idx] = 1;

      recurse_solve(letters, node->child[idx], answer, level+1, nletters,
                    used_letter, callback, data);

      used_letter[i] = 0;
    }
  }

  answer[level] = '\0';
}

/* Solves the letters game for the given letters by calling 'callback' with
   each of the words found */
void solve_letters(const char *letters,
                   void (*callback)(const char *word, void *data),
                   void *data) {
  int nletters = strlen(letters);
  char *used_letter;
  char *answer;

  used_letter = malloc(nletters);
  memset(used_letter, 0, nletters);

  answer = malloc(nletters + 1);
  memset(answer, 0, nletters + 1);

  recurse_solve(letters, dictionary, answer, 0, nletters, used_letter,
                callback, data);

  free(answer);
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
