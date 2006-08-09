/*-
 * Copyright (c) 2006 Matthias Schmidt <matthias @ staatsfeind . org>
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

/* $Id: util.c,v 1.1 2006/08/09 08:27:47 matthias Exp $ */

#include "dermob.h"

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
		default:
			mprintf("\n"); break;
	}
}

void
display_cpu_arch(int cputype)
{
        switch (cputype) {
		case CPU_TYPE_MC680x0:
			mprintf("MC680x0\n"); break;
		case CPU_TYPE_I386:
			mprintf("x86\n"); break;
		case CPU_TYPE_POWERPC:
			mprintf("PowerPC\n"); break;
		case CPU_TYPE_POWERPC64:
			mprintf("PowerPC 64\n"); break;
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