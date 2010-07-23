#Makefile for cntdn
#James Stanley 2010

CFLAGS=-Wall -g
OUT=numbers
PREFIX?=/usr

all: $(OUT)
.PHONY: all

clean:
	-rm -f $(OUT)
.PHONY: clean

install:
	install -m 0755 numbers $(DESTDIR)$(PREFIX)/bin
.PHONY: install
