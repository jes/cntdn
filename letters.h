/* Header for letters.c

   James Stanley 2010 */

#ifndef LETTERS_H_INC
#define LETTERS_H_INC

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>

#ifndef BUFLEN
#define BUFLEN 8192
#endif

typedef struct TrieNode {
  struct TrieNode *child[26];
  char end_word;
} TrieNode;

extern TrieNode *dictionary;

void load_dictionary(const char *path);

#ifndef CNTDN
/* the following is only if we're not part of the cntdn program */
void die(const char *fmt, ...);
#endif

#endif
