/* numbers - Countdown numbers game solver

   James Stanley 2010 */

#include "numbers.h"

#define ADD  0
#define SUB1 1
#define SUB2 2
#define MUL  3
#define DIV1 4
#define DIV2 5

static int sp = 0;
static int stack[32];

int numbers;

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

/* push the given values on to the output stack
   to output in form "n1 o n2 = r" */
void push_vals(int n1, int o, int n2, int r) {
  if(o == SUB2 || o == DIV2) {
    stack[sp++] = n2;
    stack[sp++] = o;
    stack[sp++] = n1;
  } else {
    stack[sp++] = n1;
    stack[sp++] = o;
    stack[sp++] = n2;
  }
  stack[sp++] = r;
}

/* prints the operands/operators stored on the stack */
void print_vals(void) {
  static char opname[6] = { '+', '-', '-', '*', '/', '/' };
  int n1, o, n2, r;

  while(sp) {
    r  = stack[--sp];
    n2 = stack[--sp];
    o  = stack[--sp];
    n1 = stack[--sp];

    printf("%d %c %d = %d\n", n1, opname[o], n2, r);
  }

  printf("Solved.\n");
}

/* recursively solve the game, up to "levels" levels of recursion */
static int recurse_solve(int levels) {
  int i, j, o;
  int k;
  int ni, nj;
  int result;

  if(levels == 0) return 0;

  for(i = 0; i < numbers - 1; i++) {
    ni = number[i];

    for(j = i + 1; j < numbers; j++) {
      nj = number[j];

      /* one number used up */
      numbers--;

      /* move everything after position j along to fill the gap */
      for(k = j; k < numbers; k++) number[k] = number[k + 1];

      for(o = 0; o < 6; o++) {
        if((o == DIV1) && (nj == 0 || ni % nj != 0)) continue;
        if((o == DIV2) && (ni == 0 || nj % ni != 0)) continue;
        if((o == SUB1) && (nj > ni)) continue;
        if((o == SUB2) && (ni > nj)) continue;

        /* store (ni ? nj) at position i
           we have to store in result as well so that when we output the
           answer the shortcut stack unwinding could have the wrong value
           in number[i] */
        number[i] = result = op(o, ni, nj);

        /* if the result is the target, we short-circuit and push values,
           otherwise solve() is called, and if it returns 1 we push values */
        if(result == target || recurse_solve(levels - 1)) {
          push_vals(ni, o, nj, result);
          return 1;
        }
      }

      /* move numbers along to make space at j */
      for(k = numbers; k > j; k--) number[k] = number[k - 1];

      /* put j back in */
      number[j] = nj;

      /* extra number present again */
      numbers++;
    }

    /* put i back in */
    number[i] = ni;
  }

  return 0;
}

/* solve the game, returning 1 if solved and 0 otherwise */
int solve(void) {
  int i;

  numbers = 6;

  /* see if one of these numbers is the answer */
  for(i = 0; i < 6; i++) {
    if(number[i] == target) return 1;
  }

  /* iteratively deepen the DFS */
  for(i = 2; i <= 6; i++) {
    if(recurse_solve(i)) return 1;
  }

  return 0;
}
