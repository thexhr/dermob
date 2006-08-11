CC = gcc
LD = gcc

RM = rm -f
INSTALL = install
STRIP = strip

CFLAGS = -c -Wall -Wno-unused -O2 -fomit-frame-pointer
LDFLAGS = -O2

#CFLAGS = -c -Wall -g3 -ggdb
#LDFLAGS = -Wall -g3 -ggdb

PREFIX = /usr
SBINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man

BIN=dermob
OBJS= \
	util.o \
	dermob.o \
	dermob-cli.o


all: $(BIN)

dermob: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(BIN)
.c.o:
	$(CC) $(CFLAGS) $*.c

install: $(BIN)
	$(INSTALL) -m 755 $(BIN) $(SBINDIR)/
#	$(INSTALL) -m 644 $(BIN).1 $(MANDIR)/man1/

uninstall:
	$(RM) $(SBINDIR)/$(BIN)
#	$(RM) $(MANDIR)/man1/$(BIN).1
	
clean:
	$(RM) $(BIN) $(OBJS)

