/* String lists for cntdn

   James Stanley 2010 */

#include "cntdn.h"

/* random player names */
static const char *name_string[] = {
  "Augustus", "Billy", "Dwayne", "Edmond", "Eric", "Jed", "Wayne", "Zeb"
};
static int num_names = sizeof(name_string) / sizeof(*name_string);

/* return a random player name */
const char *get_name(void) {
  return name_string[rand() % num_names];
}
