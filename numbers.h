/* Header for numbers.c

   James Stanley 2010 */

#ifndef NUMBERS_H_INC
#define NUMBERS_H_INC

#include <stdlib.h>
#include <stdio.h>

/* store the operands, operator, and result for one stack frame */
typedef struct {
  int n1, o, n2, r;
} Frame;

extern int number[6];
extern int target;

extern int numbers;

int solve(void);
void print_vals(Frame *f1, Frame *f2);

#endif
