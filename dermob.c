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

/* $Id: dermob.c,v 1.24 2006/08/12 10:51:28 matthias Exp $ */

#include "dermob.h"
#include "mach.h"
#include "defs.h"

/*
 * Analyse a possible fat header or return silently
 */
int
analyse_fat_header(char *buffer, int *offset, struct fat_header *rfh)
{
	struct fat_header *fh;
	char *ptr;
	
	fh = malloc(sizeof(*fh));
	ptr = buffer;

	memcpy(fh, ptr, sizeof(*fh));

	/* No universal binary */
	if (fh->magic != FAT_CIGAM && fh->magic != FAT_MAGIC) {
		free(fh);
		rfh = NULL;
		return(-1);
	}
	
	if (fh->magic == FAT_MAGIC)
		bo_b = LE;
	else if (fh->magic == FAT_CIGAM)
		bo_b = BE;
	
	*offset += sizeof(*fh);
	memcpy(rfh, fh, sizeof(*fh));
	
	free(fh);
	return(1);
}

int
analyse_fat_arch(char *buffer, int *offset, struct fat_arch *rfa)
{
	char *ptr;
	ptr = buffer;
	ptr += *offset;
	
	memcpy(rfa, ptr, sizeof(*rfa));
	*offset += sizeof(*rfa);
	
	return(0);
}

/*
 * Parse the mach-o header, set the correct byte order ot the binary and
 * determine the number of load commands.
 */
int
analyse_mo_header(char *buffer, int *offset, struct mach_header *rmh)
{
	struct mach_header *mh;
	char *ptr;

	mh = malloc(sizeof(*mh));
	ptr = buffer;

	if (offset > 0) 
		ptr += *offset;

	memcpy(mh, ptr, sizeof(*mh));

	/* No valid mach-o binary */
	if (mh->magic != MH_MAGIC && mh->magic != MH_CIGAM) {
		rmh = NULL;
		free(mh);
		return(-1);
	}

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
	*offset += sizeof(*mh);
	
	memcpy(rmh, mh, sizeof(*rmh));
	free(mh);
	
	return(0);
}

/*
 * Analyse all load commands
 */
void
analyse_load_command(char *buffer, int *offset, struct load_command *rld)
{
	char *ptr;
	
	ptr = buffer;
	ptr += *offset;

	memcpy(rld, ptr, sizeof(*rld));
}

/*
 * Examine the different sections and display various information.
 */
void
examine_section(char *buffer, int *offset, struct section *rsec)
{
	char *ptr;

	ptr = buffer;
	ptr += *offset;

	memcpy(rsec, ptr, sizeof(*rsec));
	*offset += sizeof(*rsec);
}

/*
 * Examine the different segments and display various information.
 */
int
examine_segmet(char *buffer, int *offset, int cmd, int cmdsize, int *nofx)
{
	struct segment_command *sc;
	struct symtab_command *symc;
	struct dylib_command *dly;
	struct dylinker_command *dlnk;
	struct dysymtab_command *dsym;
	struct twolevel_hints_command *two;
	time_t timev;
	int ret = 0;
	char *ptr;
	
	ptr = buffer;
	ptr += *offset;
	
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
			//*offset += sizeof(*sc);
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
			//*offset += sizeof(*symc);
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
			//*offset += sizeof(*dly);
			ret = sizeof(*dly);
			free(dly);
			break;
		case LC_LOAD_DYLINKER:
			dlnk = malloc(sizeof(*dlnk));
			memcpy(dlnk, ptr, sizeof(*dlnk));
			mprintf("  Name:		%s\n", ptr+swapi(dlnk->name.offset));
			//*offset += sizeof(*dlnk);
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
			//*offset += sizeof(*dsym);
			ret = sizeof(*dsym);
			free(dsym);
			break;
		case LC_TWOLEVEL_HINTS:
			two = malloc(sizeof(*two));
			memcpy(two, ptr, sizeof(*two));
			mprintf("  Offset:		%d\n", swapi(two->offset));
			mprintf("  No of 2level hints:	%d\n", swapi(two->nhints));
			//*offset += sizeof(*two);			
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

	printf("Starting at address 0x%x offset %d size %d\n", addr, offset, size);
	
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
