/* numbers - Countdown numbers game solver

   James Stanley 2010 */

#include "numbers.h"

#define ADD  0
#define SUB1 1
#define SUB2 2
#define MUL  3
#define DIV1 4
#define DIV2 5

int numbers;
int num_levels;

/* apply operation o to n1 and n2 and return the result
   fails silently on division by zero or invalid operator */
int op(int o, int n1, int n2) {
  switch(o) {
  case ADD : return n1 + n2;
  case SUB1: return n1 - n2;
  case SUB2: return n2 - n1;
  case MUL : return n1 * n2;
  case DIV1: return n1 / n2;
  case DIV2: return n2 / n1;
  }

  return 0;
}

/* prints the operands/operators stored on the call stack
   WARNING: deep magic. this function calculates the size of one recurse_solve()
   stack frame by looking at the difference in address between the two pointers.
   it then prints out the values in a sane order by obtaining pointers to the
   Frame structure in each stack frame in turn. */
void print_vals(Frame *f1, Frame *f2) {
  static char opname[6] = { '+', '-', '-', '*', '/', '/' };
  int frame_size = f2 - f1;
  Frame *f;
  int i;

  for(i = num_levels - 1; i >= 0; i--) {
    f = f1 + i * frame_size;

    if(f->o == SUB2 || f->o == DIV2)
      printf("%d %c %d = %d\n", f->n2, opname[f->o], f->n1, f->r);
    else
      printf("%d %c %d = %d\n", f->n1, opname[f->o], f->n2, f->r);
  }
}

/* recursively solve the game, up to "levels" levels of recursion */
static int recurse_solve(Frame *parent_f, int levels) {
  int i, j;
  int k;
  Frame f;

  if(levels == 0) return 0;

  for(i = 0; i < numbers - 1; i++) {
    f.n1 = number[i];

    for(j = i + 1; j < numbers; j++) {
      f.n2 = number[j];

      /* one number used up */
      numbers--;

      /* move everything after position j along to fill the gap */
      for(k = j; k < numbers; k++) number[k] = number[k + 1];

      for(f.o = 0; f.o < 6; f.o++) {
        if((f.o == DIV1) && (f.n2 == 0 || f.n1 % f.n2 != 0)) continue;
        if((f.o == DIV2) && (f.n1 == 0 || f.n2 % f.n1 != 0)) continue;
        if((f.o == SUB1) && (f.n2 > f.n1)) continue;
        if((f.o == SUB2) && (f.n1 > f.n2)) continue;

        /* store (f.n1 ? f.n2) at position i */
        number[i] = f.r = op(f.o, f.n1, f.n2);

        /* if solved, print solution and return */
        if(f.r == target) {
          print_vals(&f, parent_f);
          return 1;
        }
        
        /* recurse, and if it gets solved return */
        if(recurse_solve(&f, levels - 1))
          return 1;
      }

      /* move numbers along to make space at j */
      for(k = numbers; k > j; k--) number[k] = number[k - 1];

      /* put j back in */
      number[j] = f.n2;

      /* extra number present again */
      numbers++;
    }

    /* put i back in */
    number[i] = f.n1;
  }

  return 0;
}

/* solve the game, returning 1 if solved and 0 otherwise */
int solve(void) {
  int i;

  numbers = 6;

  /* see if one of these numbers is the answer */
  for(i = 0; i < 6; i++) {
    if(number[i] == target) {
      printf("%d = %d\n", number[i], target);
      return 1;
    }
  }

  /* iteratively deepen the DFS */
  for(num_levels = 2; num_levels <= 6; num_levels++) {
    if(recurse_solve(NULL, num_levels)) return 1;
  }

  return 0;
}
