/* Header for cntdn

   James Stanley 2010 */

#ifndef CNTDN_H_INC
#define CNTDN_H_INC

#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>

#define BUFLEN 4096

/* cntdn.c */
void die(const char *reason);
char *get_line(void);

/* opts.c */
extern char *program_name;

extern int players;
extern int timer;
extern char *format;
extern int nocolour;

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

extern char *letter_colour;
extern char *colour_off;
extern char *colour[];
extern const int num_colours;
extern char *pres_colour[];

const char *get_name(void);
char get_vowel(void);
char get_consonant(void);

/* games.c */
void letters_round(void);
void numbers_round(void);
void teatime_teaser(void);
void conundrum(void);

/* dictcorner.c */
void origin_of_words(void);
void guest_chat(void);
int valid_word(const char *word);

#endif
