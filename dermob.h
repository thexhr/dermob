#ifndef __DERMOB_H
#define __DERMOB_H

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

#define BE	0
#define LE	1

int trigger;
int text_offset;
int text_size;
int text_addr;
int dynamic;
int dyn_display;
int cpu;
int bo_a;
int bo_b;

/* util.c */
extern void display_cmd_name(int);
extern void display_cpu_arch(int);
extern void mprintf(const char *fmt, ...);
extern int get_cpu_information();
extern int get_bo_information();

/* dermob.c */
extern int examine_segmet(char *buffer, char *ptr, int cmd, int cmdsize, int *nofx);
extern void examine_section(char *buffer, char *ptr, int val, int nofx);
extern int swapi(int i);

#endif