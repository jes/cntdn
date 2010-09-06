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

static Number nil_number;
static Number *nil = &nil_number;

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
static int recurse_solve(Number *list, int levels) {
  int o;
  Number *ni, *nj;
  int result;
  int i, j;

  if(levels == 0) return 0;

  for(ni = list->next; ni->next != nil; ni = ni->next) {
    i = ni->val;

    for(nj = ni->next; nj != nil; nj = nj->next) {
      j = nj->val;

      /* remove nj from the list */
      nj->prev->next = nj->next;
      nj->next->prev = nj->prev;

      for(o = 0; o < 6; o++) {
        if((o == DIV1) && (j == 0 || i % j != 0)) continue;
        if((o == DIV2) && (i == 0 || j % i != 0)) continue;

        /* store (i ? j) in list at i's position
           we have to store in result as well so that when we output the
           answer the shortcut stack unwinding could have the wrong value
           in ni->val */
        ni->val = result = op(o, i, j);

        /* don't want negative values at any point */
        if(result < 0) continue;

        /* if the result is the target, we short-circuit and push values,
           otherwise solve() is called, and if it returns 1 we push values */
        if(result == target || recurse_solve(list, levels - 1)) {
          push_vals(i, o, j, result);
          return 1;
        }
      }

      /* put nj back in */
      nj->prev->next = nj;
      nj->next->prev = nj;
    }

    /* put i back in */
    ni->val = i;
  }

  return 0;
}

/* solve the game, returning 1 if solved and 0 otherwise */
int solve(void) {
  int i;
  Number *list = nil;
  Number *n;

  /* see if one of these numbers is the answer */
  for(i = 0; i < 6; i++) {
    if(number[i] == target) return 1;
  }

  /* initialise the list */
  list->next = list;
  list->prev = list;

  /* add the numbers to the list */
  for(i = 0; i < 6; i++) {
    n = malloc(sizeof(Number));
    n->val = number[i];
    n->next = list;
    n->prev = list->prev;
    list->prev->next = n;
    list->prev = n;
  }

  /* iteratively deepen the DFS */
  for(i = 2; i <= 6; i++) {
    if(recurse_solve(list, i)) return 1;
  }

  return 0;
}
