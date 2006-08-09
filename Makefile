CC = gcc
LD = gcc

RM = rm -f
INSTALL = install
STRIP = strip

#CFLAGS = -c -Wall -Wno-unused -Os -fomit-frame-pointer
#LDFLAGS = -Os

CFLAGS = -c -Wall -g3 -ggdb
LDFLAGS = -Wall -g3 -ggdb

PREFIX = /usr
SBINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/local/man

BIN=dermob
OBJS= \
	util.o \
	dermob.o


all: $(BIN)

dermob: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(BIN)
.c.o:
	$(CC) $(CFLAGS) $*.c

install: $(BIN)
#	$(INSTALL) -m 755 $(BIN) $(SBINDIR)/
#	$(INSTALL) -m 644 $(BIN).1 $(MANDIR)/man1/

uninstall:
#	$(RM) $(SBINDIR)/$(BIN) $(MANDIR)/man1/$(BIN).1
	
strip:
	$(STRIP) -R .command -R .note $(BIN)

clean:
	$(RM) $(BIN) $(OBJS)

