#ifndef __DERMOB_H
#define __DERMOB_H

#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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

// Trigger text display on/off
int trigger;
// Offset, size and address of __TEXT, __text section
int text_offset;
int text_size;
int text_addr;
int data_offset;
int data_size;
int data_addr;

int offset_moh;

// display shared libraries
int dynamic;
int dyn_display;

// Machine CPU type
int cpu;
// Machine byte order
int bo_a;
// Binary byte order
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
extern unsigned int swapi(unsigned int i);
extern void analyse_load_command(char *buffer, int offset, int ncmds);
extern void display_buffer(char *buffer, int addr, int offset, int size);
extern int analyse_fat_header(char *buffer, int *offset);
extern int analyse_mo_header(char *buffer, int *offset, int *ncmds);
#endif
