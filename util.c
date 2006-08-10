/*-
 * Copyright (c) 2006 Matthias Schmidt <xhr @ staatsfeind . org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/* $Id: util.c,v 1.6 2006/08/10 15:32:12 matthias Exp $ */

#include "dermob.h"

#define swap_bo(i) \
	(((i & 0xFF000000) >> 24) | \
	((i & 0x00FF0000) >> 8) | \
	((i & 0x0000FF00) << 8) | \
	((i & 0x000000FF) << 24));

void
display_cmd_name(int cmd)
{
	switch(cmd) {
		case LC_SEGMENT:
			mprintf("LC_SEGMENT\n"); break;
		case LC_SYMTAB:
			mprintf("LC_SYMTAB\n"); break;
		case LC_LOAD_DYLIB:
			mprintf("LC_LOAD_DYLIB\n"); break;
		case LC_LOAD_DYLINKER:
			mprintf("LC_LOAD_DYLINKER\n"); break;
		case LC_DYSYMTAB:
			mprintf("LC_DYSYMTAB\n"); break;
		case LC_UNIXTHREAD:
			mprintf("LC_UNIXTHREAD\n"); break;
		case LC_TWOLEVEL_HINTS:
			mprintf("LC_TWOLEVEL_HINTS\n"); break;		
		default:
			mprintf("\n"); break;
	}
}

void
display_cpu_arch(int cputype)
{
        switch (cputype) {
		case CPU_TYPE_MC680x0:
			mprintf("MC680x0"); break;
		case CPU_TYPE_I386:
			mprintf("x86"); break;
		case CPU_TYPE_POWERPC:
			mprintf("PowerPC"); break;
		case CPU_TYPE_POWERPC64:
			mprintf("PowerPC 64"); break;
		default:
			mprintf("\n"); break;
	}
}

void
mprintf(const char *fmt, ...)
{
        va_list ap;
        
	if (trigger == 1)
		return;

	va_start(ap, fmt);
	vfprintf(stdout, fmt, ap);
	va_end(ap);
}

int
get_cpu_information()
{
	const NXArchInfo *na;
	int ret = 0;
	
	if ((na = NXGetLocalArchInfo()) == NULL)  {
		printf("CPU architecture unknown.\n");
		return(ret);
	}
	
	ret = na->cputype;		
	return(ret);
}

int
get_bo_information()
{
	const NXArchInfo *na;
	int ret = 0;
	
	if ((na = NXGetLocalArchInfo()) == NULL)  {
		printf("CPU architecture unknown.\n");
		return(ret);
	}
	
	ret = na->byteorder;
	return(ret);
}

unsigned int
swapi(unsigned int i)
{
	if (bo_a == NX_LittleEndian && bo_b == BE)
		return swap_bo(i);
	
	return(i);
}