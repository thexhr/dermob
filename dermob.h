#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/uio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <arpa/inet.h>

#include <mach-o/arch.h>
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <mach/machine.h>

int trigger = 0;
int text_offset = 0;
int text_size = 0;
int text_addr = 0;

//extern void mprintf(int trigger, char *fmt, ...);
//extern void mprintf(const char *fmt, ...);

extern int examine_segmet(char *buffer, char *ptr, int cmd, int cmdsize, int *nofx);
extern void examine_section(char *buffer, char *ptr, int val, int nofx);