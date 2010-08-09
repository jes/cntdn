/* Header for cntdn

   James Stanley 2010 */

#ifndef CNTDN_H_INC
#define CNTDN_H_INC

#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/* cntdn.c */
void die(const char *reason);

/* opts.c */
extern char *program_name;

extern int players;

void parse_opts(int argc, char **argv);

/* players.c */
typedef struct {
  char *name;
} Player;
extern Player *player;

void make_players(void);

/* strings.c */
const char *get_name(void);

#endif
