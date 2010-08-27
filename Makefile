#Makefile for cntdn
#James Stanley 2010

CFLAGS=-Wall -g -O3
CNTDN_CFLAGS=$(CFLAGS) -DCNTDN
OBJS=cntdn.o opts.o players.o strings.o games.o dictcorner.o letters.o numbers.o
PREFIX?=/usr

all: cntdn numbers letters
.PHONY: all

cntdn: $(OBJS)
	$(CC) -o cntdn $(OBJS)

numbers: numbers.c
	$(CC) -o numbers numbers.c $(CFLAGS)

letters: letters.c
	$(CC) -o letters letters.c $(CFLAGS)

%.o: %.c
	$(CC) $(CNTDN_CFLAGS)  -o $@ -c $<

clean:
	-rm -f numbers letters cntdn $(OBJS)
.PHONY: clean

install:
	install -m 0755 numbers $(DESTDIR)$(PREFIX)/bin
	install -m 0755 letters $(DESTDIR)$(PREFIX)/bin
	install -m 0755 cntdn   $(DESTDIR)$(PREFIX)/bin
.PHONY: install
