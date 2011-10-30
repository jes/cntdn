/* Header for letters.c

   James Stanley 2010 */

#ifndef LETTERS_H_INC
#define LETTERS_H_INC

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include "config.h"

#ifndef BUFLEN
#define BUFLEN 8192
#endif

typedef struct TrieNode {
  struct TrieNode *child[26];
  char end_word;
} TrieNode;

extern TrieNode *dictionary;

extern int ignore_invalid;
extern int minletters;

void load_dictionary(const char *path, int maxlen);
void solve_letters(const char *letters,
                   void (*callback)(const char *word, void *data), void *data);
int word_in_dictionary(const char *word);
int can_make_word(const char *word, const char *letters);
void die(const char *fmt, ...);

#endif
