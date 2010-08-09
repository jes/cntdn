#Makefile for cntdn
#James Stanley 2010

CFLAGS=-Wall -g
OUT=numbers cntdn
OBJS=cntdn.o opts.o players.o strings.o
PREFIX?=/usr

all: $(OUT)
.PHONY: all

cntdn: $(OBJS)

clean:
	-rm -f $(OUT) $(OBJS)
.PHONY: clean

install:
	install -m 0755 numbers $(DESTDIR)$(PREFIX)/bin
.PHONY: install
