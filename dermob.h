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
#include <sys/sysctl.h>
#include <sys/types.h>

#include <arpa/inet.h>

#define BE	0
#define LE	1

// Trigger text display on/off
int trigger;
// Offset, size and address of __TEXT, __text section
int text_offset;
int text_size;
int text_addr;
// Offset, size and address of __DATA, __data section
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

#endif
