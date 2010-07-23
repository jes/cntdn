/*	numbers - Countdown numbers game solver	*/
/*	James Stanley 2010	*/

#include <stdlib.h>
#include <stdio.h>

#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3

int sp = 0;
int stack[32];

int number[6];
int numbers;
int target;
int level;

/*	apply operation o to n1 and n2 and return the result	*/
/*	fails silently on division by zero or invalid operator	*/
int op(int o, int n1, int n2) {
	switch(o) {
	case ADD: return n1 + n2;
	case SUB: return n1 - n2;
	case MUL: return n1 * n2;
	case DIV: return n1 / n2;
	}

	return 0;
}

/*	push the given values on to the output stack	*/
/*	to output in form "n1 o n2 = r"	*/
void push_vals(int n1, int o, int n2, int r) {
	stack[sp++] = n1;
	stack[sp++] = o;
	stack[sp++] = n2;
	stack[sp++] = r;
}

/* prints the operands/operators stored on the stack */
void print_vals(void) {
	static char	opname[4] = { '+', '-', '*', '/' };
	int	n1, o, n2, r;

	while(sp) {
		r	= stack[--sp];
		n2	= stack[--sp];
		o	= stack[--sp];
		n1	= stack[--sp];

		printf("%d %c %d = %d\n", n1, opname[o], n2, r);
	}
}

/* recursively solve the game, returning 1 if solved and 0 otherwise */
int solve(void) {
	int i, j, o;
	int k;
	int ni, nj;
	int result;

	for(i = 0; i < numbers - 1; i++) {
		ni = number[i];

		for(j = i + 1; j < numbers; j++) {
			nj = number[j];

			/* one number used up */
			numbers--;

			/* move everything after position j along to fill the gap */
			for(k = j; k < numbers; k++) number[k] = number[k + 1];

			for(o = 0; o < 4; o++) {
				if((o == DIV) && (nj == 0 || ni % nj != 0)) continue;

				/*	store (ni ? nj) at position i	*/
				/*	we have to store in result as well so that when we output the	*/
				/*	answer the shortcut stack unwinding could have the wrong value	*/
				/*	in number[i]	*/
				number[i] = result = op(o, ni, nj);

				/*	if the result is the target, we short-circuit and push values,	*/
				/*	otherwise solve() is called, and if it returns 1 we push values	*/
				if(result == target || solve()) {
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
	if(solve() == 1)	print_vals();
	else	fprintf(stderr, "No solution.\n");

	return 0;
}
