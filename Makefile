CC = gcc
LD = gcc

RM = rm -f
INSTALL = install
STRIP = strip

# Optimized Flags
#CFLAGS = -c -Wall -Wno-unused -Os -fomit-frame-pointer
#LDFLAGS = -Os

# Debug Flags
CFLAGS = -c -Wall -g3 -ggdb
LDFLAGS = -Wall -g3 -ggdb

#LDFLAGS += -static 

PREFIX = /usr
SBINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/local/man

BIN=dermob
OBJS= \
	dermob.o


all: $(BIN)

autoipd: $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(BIN)
.c.o:
	$(CC) $(CFLAGS) $*.c

install: $(BIN)
	$(INSTALL) -m 755 $(BIN) $(SBINDIR)/
#	$(INSTALL) -m 644 $(BIN).1 $(MANDIR)/man1/

uninstall:
	$(RM) $(SBINDIR)/$(BIN) $(MANDIR)/man1/$(BIN).1
	
strip:
	$(STRIP) -R .command -R .note $(BIN)

clean:
	$(RM) $(BIN) $(OBJS)

