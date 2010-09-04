/* numbers - Countdown numbers game solver

   James Stanley 2010 */

#include "numbers.h"

#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3

int sp = 0;
int stack[32];

int numbers;

/* apply operation o to n1 and n2 and return the result
   fails silently on division by zero or invalid operator */
int op(int o, int n1, int n2) {
  switch(o) {
  case ADD: return n1 + n2;
  case SUB: return n1 - n2;
  case MUL: return n1 * n2;
  case DIV: return n1 / n2;
  }

  return 0;
}

/* push the given values on to the output stack
   to output in form "n1 o n2 = r" */
void push_vals(int n1, int o, int n2, int r) {
  stack[sp++] = n1;
  stack[sp++] = o;
  stack[sp++] = n2;
  stack[sp++] = r;
}

/* prints the operands/operators stored on the stack */
void print_vals(void) {
  static char opname[4] = { '+', '-', '*', '/' };
  int n1, o, n2, r;

  while(sp) {
    r = stack[--sp];
    n2 = stack[--sp];
    o = stack[--sp];
    n1 = stack[--sp];

    printf("%d %c %d = %d\n", n1, opname[o], n2, r);
  }
}

/* prepend number to list, allocating a one-item list if necessary, and always
   returning a pointer to the start of the new list */
static Number *list_prepend(Number *list, int number) {
  Number *n;

  n = malloc(sizeof(Number));
  n->next = list;
  n->number = number;

  return n;
}

/* remove the first occurence of number from list, possibly ending up with an
   empty list (NULL). the function takes care of freeing memory and always
   returns a pointer to the new list */
static Number *list_remove(Number *list, int number) {
  Number *n, *n2;

  if(!list) return NULL;

  if(list->number == number) {/* first item is number */
    n2 = list->next;
    free(list);
    list = n2;
  } else {/* search for number in the remaining items */
    for(n = list; n->next && n->next->number != number; n = n->next);

    if(n->next && n->next->number == number) {/* number has been found */
      n2 = n->next;
      n->next = n->next->next;
      free(n2);
    }
  }

  return list;
}

/* produce a copy of list and return a pointer to the first element */
static Number *copy_list(Number *list) {
  Number *n, *n2;
  Number *copy = NULL;

  if(!list) return NULL;

  for(n = list; n; n = n->next) {
    if(copy) {
      n2->next = malloc(sizeof(Number));
      n2 = n2->next;
    } else {
      copy = malloc(sizeof(Number));
      n2 = copy;
    }

    n2->number = n->number;
    n2->next = NULL;
  }

  return copy;
}

/* free entire list */
static void delete_list(Number *list) {
  Number *lnext;

  while(list) {
    lnext = list->next;
    free(list);
    list = lnext;
  }
}

/* make a copy of list, with number prepended to it */
static Number *copy_list_prepend(Number *list, int number) {
  Number *copy;

  copy = copy_list(list);
  copy = list_prepend(copy, number);

  return copy;
}

/* make a copy of list, with the first occurence of number removed */
static Number *copy_list_remove(Number *list, int number) {
  Number *copy;

  copy = copy_list(list);
  copy = list_remove(copy, number);

  return copy;
}

/* recursively solve the game, up to "levels" levels of recursion */
static int recurse_solve(Number *list, int levels) {
  int o;
  int result;
  Number *ni, *nj;
  Number *ilist, *jlist, *nlist;

  if(levels == 0) return 0;

  for(ni = list; ni; ni = ni->next) {
    /* remove ni from the list of numbers */
    ilist = copy_list_remove(list, ni->number);

    for(nj = ilist; nj; nj = nj->next) {
      if(ni->number == nj->number) continue;

      /* remove nj from the list of numbers */
      jlist = copy_list_remove(ilist, nj->number);

      for(o = 0; o < 4; o++) {
        if((o == DIV) && (nj->number == 0 || ni->number % nj->number != 0))
          continue;

        /* store the result of this operation */
        result = op(o, ni->number, nj->number);
        nlist = copy_list_prepend(jlist, result);

        /* if the result is the target, we short-circuit and push values,
           otherwise recurse_solve() is called, and if it returns 1 we push
           values */
        if(result == target || recurse_solve(nlist, levels - 1)) {
          push_vals(ni->number, o, nj->number, result);
          return 1;
        }

        delete_list(nlist);
      }

      delete_list(jlist);
    }

    delete_list(ilist);
  }

  return 0;
}

/* solve the game, returning 1 if solved and 0 otherwise */
int solve(void) {
  int i;
  Number *list = NULL;

  numbers = 6;

  /* add the numbers to the list used by recurse_solve() */
  for(i = 0; i < 6; i++) {
    list = list_prepend(list, number[i]);
  }

  /* iteratively deepen the DFS */
  for(i = 1; i <= 6; i++) {
    if(recurse_solve(list, i)) return 1;
  }

  return 0;
}
