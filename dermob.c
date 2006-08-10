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

/* $Id: dermob.c,v 1.21 2006/08/10 16:50:07 matthias Exp $ */

#include "dermob.h"
#include "mach.h"

/*
 * Analyse a possible fat header or return silently
 */
int
analyse_fat_header(char *buffer, int *offset)
{
	struct fat_header *fh;
	struct fat_arch *fa;
	char *ptr;
	int i, ret = 0;
	
	fh = malloc(sizeof(*fh));
	fa = malloc(sizeof(*fa));
	ptr = buffer;

	memcpy(fh, ptr, sizeof(*fh));

	/* No universal binary */
	if (fh->magic != FAT_CIGAM && fh->magic != FAT_MAGIC) {
		free(fa);
		free(fh);
		return(ret);
	}
	
	if (fh->magic == FAT_MAGIC)
		bo_b = LE;
	else if (fh->magic == FAT_CIGAM)
		bo_b = BE;
		
	mprintf("Universal Binary header starting at 0x%.08x\n\n", *offset);
	mprintf(" Magic:		0x%x\n", swapi(fh->magic));

	ptr += sizeof(*fh);
	
	for (i = 0; i < swapi(fh->nfat_arch); i++) {
		memcpy(fa, ptr, sizeof(*fa));
		mprintf(" Architecture %d\n", i);
		mprintf("   CPU Type:	");
		display_cpu_arch(swapi(fa->cputype));
		mprintf("\n");
		mprintf("   Subtype:	%d\n", swapi(fa->cpusubtype));
		mprintf("   Offest:	%d\n", swapi(fa->offset));
		mprintf("   Size:	%d\n", swapi(fa->size));
		mprintf("   Align:	%d\n", swapi(fa->align));
		mprintf("\n");
		
		if (swapi(fa->cputype) == cpu)
			*offset = swapi(fa->offset);
		ret++;
		ptr += sizeof(*fa);
	}

	free(fa);
	free(fh);
	
	return(ret);
}

/*
 * Parse the mach-o header, set the correct byte order ot the binary and
 * determine the number of load commands.
 */
int
analyse_mo_header(char *buffer, int *offset, int *ncmds)
{
	struct mach_header *mh;
	char *ptr;
	int ret = 0;
	
	mh = malloc(sizeof(*mh));
	ptr = buffer;	
	if (offset > 0) 
		ptr += *offset;

	memcpy(mh, ptr, sizeof(*mh));
	
	/* No valid mach-o binary */
	if (mh->magic != MH_MAGIC && mh->magic != MH_CIGAM)
		return(ret);

	/* Determine the correct byte order */
	if (mh->magic == MH_MAGIC && bo_a == LE)
		bo_b = LE;
	else if (mh->magic == MH_CIGAM && bo_a == LE)
		bo_b = BE;
	else if (mh->magic == MH_MAGIC && bo_a != LE)
		bo_b = BE;
	else if (mh->magic == MH_CIGAM && bo_a != LE)
		bo_b = LE;

	offset_moh = *offset;
	mprintf("Mach-o header starting at 0x%.08x\n\n", *offset);
	
	mprintf(" Magic:		0x%x\n", swapi(mh->magic));
	mprintf(" CPU Type:	");
	display_cpu_arch(swapi(mh->cputype));
	ret = mh->cputype;
	mprintf("\n");	
	mprintf(" Subtype:	%d\n", swapi(mh->cpusubtype));
	mprintf(" Filetype:	0x%x\n", swapi(mh->filetype));
	mprintf(" No load cmds:	%d cmds\n", swapi(mh->ncmds));
	mprintf(" Size of cmds:	%d bytes\n", swapi(mh->sizeofcmds));
	mprintf(" Flags:		0x%.08x\n", swapi(mh->flags));
	mprintf("\n");
	
	*ncmds = swapi(mh->ncmds);
	*offset += sizeof(*mh);
	
	free(mh);
	
	return(ret);
}

/*
 * Analyse all load commands
 */
void
analyse_load_command(char *buffer, int offset, int ncmds)
{
	struct load_command *ld;
	char *ptr;
	int i, nofx = 0, val = 0;
	
	ld = malloc(sizeof(*ld));
	
	ptr = buffer;
	if (offset > 0) 
		ptr += offset;
	
	mprintf("Load commands starting at 0x%.08x\n\n", offset);
	
	for (i = 0; i < ncmds; i++) {
		memcpy(ld, ptr, sizeof(*ld));
		mprintf("Load Command %d\n", i);
		mprintf("  Command:	");
		display_cmd_name(swapi(ld->cmd));
		mprintf("  Command size:	%d bytes\n", swapi(ld->cmdsize));
	
		offset += swapi(ld->cmdsize);
		val = examine_segmet(buffer, ptr, swapi(ld->cmd), swapi(ld->cmdsize), &nofx);
		
		/* The segment contains sections */
		if (nofx > 0) 
			examine_section(buffer, ptr, val, nofx);
			
		mprintf("\n");
		
		ptr += swapi(ld->cmdsize);
		nofx = 0;
	}

	free(ld);
}

/*
 * Examine the different sections and display various information.
 */
void
examine_section(char *buffer, char *ptr, int val, int nofx)
{
	struct section *sec;
	int j,foo=0;
	
	sec = malloc(sizeof(*sec));
		
	for (j = 0; j < nofx; j++) {
		if (j == 0) ptr += val;
		memcpy(sec, ptr, sizeof(*sec));
		mprintf("Section %d\n", j);
		mprintf("    Sectname:	%s\n", sec->sectname);
		if ((memcmp(sec->segname, "__TEXT", 7) == 0) &&
		    (memcmp(sec->sectname, "__text", 7) == 0)) {
			text_addr = swapi(sec->addr);
			text_size = swapi(sec->size);
			text_offset = swapi(sec->offset);
		}
		if ((memcmp(sec->segname, "__OBJC", 6) == 0) &&
		    (memcmp(sec->sectname, "__class", 6) == 0) && foo != 1) {
			printf("fffff\n");
			data_addr = swapi(sec->addr);
			data_size = swapi(sec->size);
			data_offset = swapi(sec->offset);
			foo=1;
		}
		
		mprintf("    VM addr:	0x%.08x\n", swapi(sec->addr));
		mprintf("    VM size:	%d bytes\n", swapi(sec->size));
		mprintf("    Offset:	%d\n", swapi(sec->offset));
		mprintf("\n");
		ptr += sizeof(*sec);
	}

	free(sec);
}

/*
 * Examine the different segments and display various information.
 */
int
examine_segmet(char *buffer, char *ptr, int cmd, int cmdsize, int *nofx)
{
	struct segment_command *sc;
	struct symtab_command *symc;
	struct dylib_command *dly;
	struct dylinker_command *dlnk;
	struct dysymtab_command *dsym;
	struct twolevel_hints_command *two;
	time_t timev;
	int ret = 0;
	
	switch(cmd) {
		case LC_SEGMENT:
			sc = malloc(sizeof(*sc));
			memcpy(sc, ptr, sizeof(*sc));
			mprintf("  Name:		%s\n", sc->segname);
			mprintf("  VM addr:	0x%.08x\n", swapi(sc->vmaddr));
			mprintf("  VM size:	0x%.08x\n", swapi(sc->vmsize));
			mprintf("  VM size:	0x%.08x\n", swapi(sc->vmsize));
			mprintf("  File offset:	0x%.08x\n", swapi(sc->fileoff));
			mprintf("  File size:	%d bytes\n", swapi(sc->filesize));
			mprintf("  Max prot:	0x%.08x\n", swapi(sc->maxprot));
			mprintf("  Init prot:	0x%.08x\n", swapi(sc->initprot));
			mprintf("  No of sects:	%d\n", swapi(sc->nsects));
			mprintf("  Flags:	0x%.08x\n", swapi(sc->flags));
			*nofx = swapi(sc->nsects);
			ret = sizeof(*sc);
			free(sc);
			break;
		case LC_SYMTAB:
			symc = malloc(sizeof(*symc));
			memcpy(symc, ptr, sizeof(*symc));
			mprintf("  Symbol table offset:	%d bytes\n", swapi(symc->symoff));
			mprintf("  Symbol table entries:	%d\n", swapi(symc->nsyms));
			mprintf("  String table offset:	%d bytes\n", swapi(symc->stroff));
			mprintf("  String table size:	%d bytes\n", swapi(symc->strsize));
			ret = sizeof(*symc);
			free(symc);
			break;
		case LC_LOAD_DYLIB:
			dynamic = 1;
			dly = malloc(sizeof(*dly));
			memcpy(dly, ptr, sizeof(*dly));
			if (dyn_display < 1) {
				mprintf("  Name:			%s\n", ptr+swapi(dly->dylib.name.offset));
				timev = swapi(dly->dylib.timestamp);
				mprintf("  Timestamp:		%s", ctime(&timev));
				mprintf("  Current version:	0x%x\n", swapi(dly->dylib.current_version));
				mprintf("  Compat version:	0x%x\n", swapi(dly->dylib.compatibility_version));
			} else {
				trigger = 0;
				mprintf("   + %s\n", ptr+swapi(dly->dylib.name.offset));
				trigger = 1;
			}
			
			ret = sizeof(*dly);
			free(dly);
			break;
		case LC_LOAD_DYLINKER:
			dlnk = malloc(sizeof(*dlnk));
			memcpy(dlnk, ptr, sizeof(*dlnk));
			mprintf("  Name:		%s\n", ptr+swapi(dlnk->name.offset));
			ret = sizeof(*dlnk);
			free(dlnk);
			break;
		case LC_DYSYMTAB:
			dsym = malloc(sizeof(*dsym));
			memcpy(dsym, ptr, sizeof(*dsym));
			mprintf("  ilocalsym:		%d\n", swapi(dsym->ilocalsym));
			mprintf("  nlocalsym:		%d\n", swapi(dsym->nlocalsym));
			mprintf("  iextdefsym:		%d\n", swapi(dsym->iextdefsym));
			mprintf("  nextdefsym:		%d\n", swapi(dsym->nextdefsym));
			mprintf("  iundefsym:		%d\n", swapi(dsym->iundefsym));
			mprintf("  nundefsym:		%d\n", swapi(dsym->nundefsym));
			mprintf("  tocoff:		%d\n", swapi(dsym->tocoff));
			mprintf("  ntoc:			%d\n", swapi(dsym->ntoc));
			mprintf("  modtaboff:		%d\n", swapi(dsym->modtaboff));
			mprintf("  nmodtab:		%d\n", swapi(dsym->nmodtab));
			mprintf("  extrefsymoff:		%d\n", swapi(dsym->extrefsymoff));
			mprintf("  nextrefsyms:		%d\n", swapi(dsym->nextrefsyms));
			mprintf("  indirectsymoff:	%d\n", swapi(dsym->indirectsymoff));
			mprintf("  nindirectsyms:	%d\n", swapi(dsym->nindirectsyms));
			mprintf("  extreloff:		%d\n", swapi(dsym->extreloff));
			mprintf("  nextrel:		%d\n", swapi(dsym->nextrel));
			mprintf("  locreloff:		%d\n", swapi(dsym->locreloff));
			mprintf("  nlocrel:		%d\n", swapi(dsym->nlocrel));
			ret = sizeof(*dsym);
			free(dsym);
			break;
		case LC_TWOLEVEL_HINTS:
			two = malloc(sizeof(*two));
			memcpy(two, ptr, sizeof(*two));
			mprintf("  Offset:		%d\n", swapi(two->offset));
			mprintf("  No of 2level hints:	%d\n", swapi(two->nhints));			
			ret = sizeof(*two);
			free(two);
			break;
		case LC_THREAD:
		case LC_UNIXTHREAD:
			break;
		case LC_ROUTINES:
			mprintf("routines\n");
			break;
		case LC_ID_DYLIB:
			mprintf("id dylib\n");
			break;
		default:
			break;
	}
	
	return (ret);
}

/*
 * Display the __TEXT,__text section (the actual machine code) as hexadecimal
 * values on screen.
 */
void
display_buffer(char *buffer, int addr, int offset, int size)
{
	char *ptr;
	char line[17];
	int i, j=0;
	
	if (offset < 0)
		return;

	//printf("addr %x offset %d size %d\n", addr, offset, size);
	
	ptr = buffer;
	ptr += offset;
	// Skip the fat header, if necessarry
	ptr += offset_moh;
	offset += offset_moh;

	for (i = 0; i < size; i++) {
		if (j == 0) 
			printf("%.08x  ", offset+i);
		printf("%.02x ", (*ptr & 0xFF));

		if (isprint(*ptr & 0xFF))
			line[j] = (*ptr & 0xFF);
		else
			line[j] = '.';
		j++;
		if (j == 16) {
			printf(" %s\n", line);
			j = 0;
		} else if (j == 8)
			printf(" ");
		ptr += 1;
	}
	printf("\n");
}
