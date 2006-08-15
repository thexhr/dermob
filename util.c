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

/* $Id: util.c,v 1.19 2006/08/15 13:13:16 matthias Exp $ */

#include "dermob.h"
#include "mach.h"
#include "defs.h"
#include "list.h"

#define swap_bo(i) \
	(((i & 0xFF000000) >> 24) | \
	((i & 0x00FF0000) >> 8) | \
	((i & 0x0000FF00) << 8) | \
	((i & 0x000000FF) << 24))

int
display_fat_header(char *buffer, int *roffset)
{
	struct fat_header *fh;
	struct fat_arch *fa;
	int offset = 0, i, narch;
	
	fh = malloc(sizeof(*fh));

	if (analyse_fat_header(buffer, &offset, fh) < 0) {
		free(fh);
		return(-1);
	}
	
	list_insert_node(lst, fh, 0x1);

	narch = swapi(fh->nfat_arch);
	for (i = 0; i < narch; i++) {
		fa = malloc(sizeof(*fa));
		analyse_fat_arch(buffer, &offset, fa);
		
		list_insert_node(lst, fa, 0x2);
		
		if (swapi(fa->size) > size) {
			printf("Malformed universal binary.  Size for one " \
			"Architecture is larger than the complete binary.\n");
			exit(1);
		}
		
		if (cpu == swapi(fa->cputype))
			*roffset = swapi(fa->offset);
	}

	return(narch);
}

int
display_mo_header(char *buffer, int *offset, int *ncmds)
{
	struct mach_header *mh;
	
	mh = malloc(sizeof(*mh));
	if (analyse_mo_header(buffer, offset, mh) < 0) {
		free(mh);
		return(-1);
	}
	
	*ncmds = swapi(mh->ncmds);
	
	list_insert_node(lst, mh, 0x3);
	
	return(1);
}

void
display_load_commands(char *buffer, int *offset, int ncmds)
{
	struct load_command *ld;
	struct section *sec;
	int i, nofx = 0, val = 0, j, offset_old;

	for (i = 0; i < ncmds; i++) {
		ld = malloc(sizeof(*ld));
		analyse_load_command(buffer, offset, ld);
		list_insert_node(lst, ld, 0x4);
		offset_old = *offset;
		val = examine_segmet(buffer, offset, swapi(ld->cmd), swapi(ld->cmdsize), &nofx);
		if (nofx > 0) {
			for (j = 0; j < nofx; j++) {
				sec = malloc(sizeof(*sec));
				// Skip the segment header
				if (j == 0) *offset += val;
				examine_section(buffer, offset, sec);
				if ((strcmp(sec->segname, "__TEXT") == 0) &&
				    (strcmp(sec->sectname, "__text") == 0)) {
					text_addr = swapi(sec->addr);
					text_size = swapi(sec->size);
					text_offset = swapi(sec->offset);
				}
				if ((strcmp(sec->segname, "__TEXT") == 0) &&
				    (strcmp(sec->sectname, "__cstring") == 0)) {
					cs_addr = swapi(sec->addr);
					cs_size = swapi(sec->size);
					cs_offset = swapi(sec->offset);
				}
				if ((strcmp(sec->segname, "__DATA") == 0) &&
				    (strcmp(sec->sectname, "__data") == 0)) {
					data_addr = swapi(sec->addr);
					data_size = swapi(sec->size);
					data_offset = swapi(sec->offset);
				}
				list_insert_node(lst, sec, 0x5);
			}
			nofx = 0;
		}
		/* XXX Have to fix this
		 *
		 * Currently val is the length of the segment.  We need val to
		 * skip the segment header, if the segment contains sections.
		 * 
		 * Would be better to adjust offset in examine_segment for all 
		 * possible segments (not currently implemented!)
		 */ 
		else
			*offset += swapi(ld->cmdsize);

		//if (*offset == offset_old)
		//	*offset += swapi(ld->cmdsize);
	}
}

void
print_lc_towlevel_hints(struct twolevel_hints_command *two)
{
	mprintf("  Offset:		%d\n", swapi(two->offset));
	mprintf("  No of 2level hints:	%d\n", swapi(two->nhints));
}

void
print_lc_dysymtab(struct dysymtab_command *dsym)
{
	mprintf("    ilocalsym:		%d\n", swapi(dsym->ilocalsym));
	mprintf("    nlocalsym:		%d\n", swapi(dsym->nlocalsym));
	mprintf("    iextdefsym:	%d\n", swapi(dsym->iextdefsym));
	mprintf("    nextdefsym:	%d\n", swapi(dsym->nextdefsym));
	mprintf("    iundefsym:		%d\n", swapi(dsym->iundefsym));
	mprintf("    nundefsym:		%d\n", swapi(dsym->nundefsym));
	mprintf("    tocoff:		%d\n", swapi(dsym->tocoff));
	mprintf("    ntoc:		%d\n", swapi(dsym->ntoc));
	mprintf("    modtaboff:		%d\n", swapi(dsym->modtaboff));
	mprintf("    nmodtab:		%d\n", swapi(dsym->nmodtab));
	mprintf("    extrefsymoff:	%d\n", swapi(dsym->extrefsymoff));
	mprintf("    nextrefsyms:	%d\n", swapi(dsym->nextrefsyms));
	mprintf("    indirectsymoff:	%d\n", swapi(dsym->indirectsymoff));
	mprintf("    nindirectsyms:	%d\n", swapi(dsym->nindirectsyms));
	mprintf("    extreloff:		%d\n", swapi(dsym->extreloff));
	mprintf("    nextrel:		%d\n", swapi(dsym->nextrel));
	mprintf("    locreloff:		%d\n", swapi(dsym->locreloff));
	mprintf("    nlocrel:		%d\n", swapi(dsym->nlocrel));
}

void
print_lc_load_dylib(struct dylib_command *dly)
{
	time_t timev;
	
	if (dyn_display < 1) {
		//mprintf("    Name:		%s\n", ptr+swapi(dly->dylib.name.offset));
		timev = swapi(dly->dylib.timestamp);
		mprintf("    Timestamp:		%s", ctime(&timev));
		mprintf("    Current version:	0x%x\n", swapi(dly->dylib.current_version));
		mprintf("    Compat version:	0x%x\n", swapi(dly->dylib.compatibility_version));
	} else {
		trigger = 0;
		//mprintf("   + %s\n", ptr+swapi(dly->dylib.name.offset));
		trigger = 1;
	}	
}

void
print_lc_symtab(struct symtab_command *symc)
{
	mprintf("    Symbol table offset:	%d bytes\n", swapi(symc->symoff));
	mprintf("    Symbol table entries:	%d\n", swapi(symc->nsyms));
	mprintf("    String table offset:	%d bytes\n", swapi(symc->stroff));
	mprintf("    String table size:		%d bytes\n", swapi(symc->strsize));	
}

void
print_lc_segment(struct segment_command *sc)
{
	mprintf("    Name:		%s\n", sc->segname);
	mprintf("    VM addr:		0x%.08x\n", swapi(sc->vmaddr));
	mprintf("    VM size:		0x%.08x\n", swapi(sc->vmsize));
	mprintf("    VM size:		0x%.08x\n", swapi(sc->vmsize));
	mprintf("    File offset:	0x%.08x\n", swapi(sc->fileoff));
	mprintf("    File size:		%d bytes\n", swapi(sc->filesize));
	mprintf("    Max prot:		0x%.08x\n", swapi(sc->maxprot));
	mprintf("    Init prot:		0x%.08x\n", swapi(sc->initprot));
	mprintf("    No of sects:	%d\n", swapi(sc->nsects));
	mprintf("    Flags:		0x%.08x\n", swapi(sc->flags));
}

void
print_section(struct section *sec)
{
	mprintf("     Sectname:	%s\n", sec->sectname);
	mprintf("     VM addr:	0x%.08x\n", swapi(sec->addr));
	mprintf("     VM size:	%d bytes\n", swapi(sec->size));
	mprintf("     Offset:	%d\n", swapi(sec->offset));
	mprintf("\n");
}

void
print_load_command(struct load_command *ld)
{
	mprintf("    Command:	");
	display_cmd_name(swapi(ld->cmd));
	mprintf("    Command size:	%d bytes\n", swapi(ld->cmdsize));
}

void
print_mo_header(struct mach_header *mh)
{
	mprintf("Magic:		0x%x\n", swapi(mh->magic));
	mprintf(" CPU Type:	");
	display_cpu_arch(swapi(mh->cputype));
	mprintf("\n");	
	mprintf(" Subtype:	%d\n", swapi(mh->cpusubtype));
	mprintf(" Filetype:	0x%x\n", swapi(mh->filetype));
	mprintf(" No load cmds:	%d cmds\n", swapi(mh->ncmds));
	mprintf(" Size of cmds:	%d bytes\n", swapi(mh->sizeofcmds));
	mprintf(" Flags:		0x%.08x\n", swapi(mh->flags));
	mprintf("\n");
}

void
print_fat_header(struct fat_header *fh)
{
	mprintf("Magic:		0x%x\n", swapi(fh->magic));
}

void
print_fat_arch(struct fat_arch *fa)
{
	/* The fat header is always stored in big-endian byte order, so we have
	 * to swap the bo, if we work on a little-endian machine.
	 */
	if (bo_a == LE) {
		mprintf("   CPU Type:	(%x) ", swap_bo(fa->cputype));
		display_cpu_arch(swap_bo(fa->cputype));
		mprintf("\n");
		mprintf("   Subtype:	%d\n", swap_bo(fa->cpusubtype));
		mprintf("   Offest:	%d\n", swap_bo(fa->offset));
		mprintf("   Size:	%d\n", swap_bo(fa->size));
		mprintf("   Align:	%d\n", swap_bo(fa->align));
	} else {
		mprintf("   CPU Type:	(%x) ", swapi(fa->cputype));
		display_cpu_arch(swapi(fa->cputype));
		mprintf("\n");
		mprintf("   Subtype:	%d\n", swapi(fa->cpusubtype));
		mprintf("   Offest:	%d\n", swapi(fa->offset));
		mprintf("   Size:	%d\n", swapi(fa->size));
		mprintf("   Align:	%d\n", swapi(fa->align));

	}
	mprintf("\n");
}

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
		case CPU_TYPE_HPPA:
			mprintf("HPPA"); break;
		case CPU_TYPE_SPARC:
			mprintf("Sparc"); break;
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
	char buf[50];
	int mib[2];
	size_t len;
		
	mib[0] = CTL_HW;
	mib[1] = HW_MACHINE;
	len = sizeof(buf);
	if (sysctl(mib, 2, &buf, &len, NULL, 0) < 0)
		errx(1, "Cannot determine local CPU type");
	
	if (strncmp(buf, "i386", 4) == 0)
		return(CPU_TYPE_X86);
	else if (strncmp(buf, "Power Macintosh", 15) == 0)
		return(CPU_TYPE_POWERPC);

	return(-1);
}

int
get_bo_information()
{
	int mib[2], bo;
	size_t len;
	
	mib[0] = CTL_HW;
	mib[1] = HW_BYTEORDER;
	len = sizeof(bo);
	if (sysctl(mib, 2, &bo, &len, NULL, 0) < 0)
		errx(1, "Cannot determine local byte order");
	
	if (bo == 1234)
		return(LE);
	else if (bo == 4321)
		return(BE);

	return(-1);
}

unsigned int
swapi(unsigned int i)
{
	unsigned int ret = i;

	if (bo_a == LE && bo_b == BE) {
		ret = swap_bo(i);
        } else if(bo_a == BE && bo_b == LE) {
		ret = swap_bo(i);
	}

        return(ret);
}
