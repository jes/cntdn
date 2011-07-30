/* Main source file for numbers solver

   James Stanley 2010 */

#include "numbers.h"

int number[6];
int target;

int main(int argc, char **argv) {
  int i;

  /* not enough arguments */
  if(argc != 8) {
    fprintf(stderr,
    "Usage: %s target n1 n2 n3 n4 n5 n6\n"
    "Where n1..6 are the 6 numbers and target is the target number.\n",
    argv[0]);

    return 1;
  }

  /* read the arguments */
  target = atoi(argv[1]);
  for(i = 0; i < 6; i++) {
    number[i] = atoi(argv[i + 2]);
  }
  numbers = 6;

  /* solve these numbers */
  if(solve() == 1) print_vals();
  else fprintf(stderr, "No solution.\n");

  return 0;
}
