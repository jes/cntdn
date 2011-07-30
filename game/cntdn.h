/* Header for cntdn

   James Stanley 2010 */

#ifndef CNTDN_H_INC
#define CNTDN_H_INC

#include <termios.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#define BUFLEN 8192

/* cntdn.c */
void die(const char *fmt, ...);
char *get_line(void);

/* opts.c */
extern char *program_name;

extern int players;
extern int timer;
extern char *format;
extern int nocolour;
extern char *dictionary_path;
extern int ignore_invalid;
extern int noflush;
extern int num_letters;

void parse_opts(int argc, char **argv);

/* players.c */
typedef struct {
  char *raw_name;
  char *name;
  int length;
  char *word;
  int answer;
  int score;
} Player;
extern Player *player;

void make_players(void);
void show_scores(int endgame);

/* strings.c */
#define JEFF   0
#define SUSIE  1
#define RACHEL 2
#define GUEST  3

extern const char *number_colour;
extern const char *letter_colour;
extern const char *colour_off;
extern const char *colour[];
extern const int num_colours;

const char *get_name(void);
void init_numbers(void);
char get_vowel(void);
char get_consonant(void);
int get_small(void);
int get_large(void);
void colours_off(void);
void speak(int presenter, const char *fmt, ...);
void slow_printf(const char *fmt, ...);

/* games.c */
extern int number[6];
extern int target;

void letters_round(void);
void numbers_round(void);
void teatime_teaser(void);
void conundrum(void);

/* dictcorner.c */
void origin_of_words(void);
void guest_chat(void);
int valid_word(int p, const char *letters);
void dict_solve(const char *letters);

/* letters.c */
#include "../letters.h"

#endif
