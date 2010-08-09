/* Option parsing for cntdn

   James Stanley 2010 */

#include "cntdn.h"

char *program_name;

int players = 1;

static struct option opts[] = {
  { "help",    no_argument,       NULL, 'h' },
  { "players", required_argument, NULL, 'p' },
  { NULL,      0,                 NULL, 0 }
};

static void usage(void) {
  printf(
  "cntdn - countdown game simulator\n"
  "Usage: %s [OPTIONS...]\n"
  "\n"
  "Options:\n"
  "  -h, --help         Display this help\n"
  "  -p, --players=N    Set number of players\n"
  "\n"
  "Report bugs to James Stanley <james@incoherency.co.uk>\n"
  , program_name);
}

/* parse command line options. this function exit()'s if appropriate */
void parse_opts(int argc, char **argv) {
  int c;

  program_name = argv[0];

  opterr = 1;

  while((c = getopt_long(argc, argv, "hp:", opts, NULL)) != -1) {
    switch(c) {
      case 'h': usage(); exit(0);       break;
      case 'p': players = atoi(optarg); break;
      default:  exit(1);                break;
    }
  }
}
