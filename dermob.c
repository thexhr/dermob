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

/* $Id: dermob.c,v 1.7 2006/08/09 10:17:32 matthias Exp $ */

#include "dermob.h"



void
usage(const char *file)
{
	printf("Usage: 	%s [-uhc] <binary>\n", file);
	printf("	-u:  Display universal header\n");
	printf("	-h:  Display mach-o header\n");
	printf("	-c:  Display complete header\n");
	printf("	-t:  Display __TEXT,__text section\n");
	exit(1);
}

/*
 * All fields are in Big-Endian Byte Order
 */
int
analyse_fat_header(char *buffer, int *offset)
{
	struct fat_header *fh;
	struct fat_arch *fa;
	const NXArchInfo *na;
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
	
	mprintf("Universal Binary header starting at 0x%.08x\n\n", *offset);
	mprintf(" Magic:		0x%x\n", htonl(fh->magic));

	ptr += sizeof(*fh);
	
	if ((na = NXGetLocalArchInfo()) == NULL)  {
		mprintf("CPU architecture unknown.\n");
		free(fa);
		free(fh);
		return(-1);
	}
	
	for (i = 0; i < htonl(fh->nfat_arch); i++) {
		memcpy(fa, ptr, sizeof(*fa));
		mprintf(" Architecture %d\n", i);
		mprintf("   CPU Type:	");
		display_cpu_arch(htonl(fa->cputype));
		mprintf("\n");
		mprintf("   Subtype:	%d\n", htonl(fa->cpusubtype));
		mprintf("   Offest:	%d\n", htonl(fa->offset));
		mprintf("   Size:	%d\n", htonl(fa->size));
		mprintf("   Align:	%d\n", htonl(fa->align));
		mprintf("\n");
		
		if (na->cputype == htonl(fa->cputype))
			*offset = htonl(fa->offset);
		
		ret++;
		ptr += sizeof(*fa);
	}

	free(fa);
	free(fh);
	
	return(ret);
}

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
	
	mprintf("Mach-o header starting at 0x%.08x\n\n", *offset);
	
	mprintf(" Magic:		0x%x\n", mh->magic);
	mprintf(" CPU Type:	");
	display_cpu_arch(mh->cputype);
	ret = mh->cputype;
	mprintf("\n");	
	mprintf(" Subtype:	%d\n", mh->cpusubtype);
	mprintf(" Filetype:	0x%x\n", mh->filetype);
	mprintf(" No load cmds:	%d cmds\n", mh->ncmds);
	mprintf(" Size of cmds:	%d bytes\n", mh->sizeofcmds);
	mprintf(" Flags:		0x%.08x\n", mh->flags);
	mprintf("\n");
	
	*ncmds = mh->ncmds;
	*offset += sizeof(*mh);
	
	free(mh);
	
	return(ret);
}

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
		display_cmd_name(ld->cmd);
		mprintf("  Command size:	%d bytes\n", ld->cmdsize);
	
		offset += ld->cmdsize;
		val = examine_segmet(buffer, ptr, ld->cmd, ld->cmdsize, &nofx);
		if (nofx > 0) 
			examine_section(buffer, ptr, val, nofx);
			
		mprintf("\n");
		
		ptr += ld->cmdsize;
		nofx = 0;
	}

	free(ld);
}

void
examine_section(char *buffer, char *ptr, int val, int nofx)
{
	struct section *sec;
	int j;
	
	sec = malloc(sizeof(*sec));
		
	for (j = 0; j < nofx; j++) {
		if (j == 0) ptr += val;
		memcpy(sec, ptr, sizeof(*sec));
		mprintf("Section %d\n", j);
		mprintf("    Sectname:	%s\n", sec->sectname);
		if ((memcmp(sec->segname, "__TEXT", 7) == 0) &&
		    (memcmp(sec->sectname, "__text", 7) == 0)) {
			text_addr = sec->addr;
			text_size = sec->size;
			text_offset = sec->offset;
		}
		
		mprintf("    VM addr:	0x%.08x\n", sec->addr);
		mprintf("    VM size:	%d bytes\n", sec->size);
		mprintf("    Offset:	%d\n", sec->offset);			
		mprintf("\n");
		ptr += sizeof(*sec);
	}

	free(sec);
}

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
			mprintf("  VM addr:	0x%.08x\n", sc->vmaddr);
			mprintf("  VM size:	0x%.08x\n", sc->vmsize);
			mprintf("  VM size:	0x%.08x\n", sc->vmsize);
			mprintf("  File offset:	0x%.08x\n", sc->fileoff);
			mprintf("  File size:	%d bytes\n", sc->filesize);
			mprintf("  Max prot:	0x%.08x\n", sc->maxprot);
			mprintf("  Init prot:	0x%.08x\n", sc->initprot);
			mprintf("  No of sects:	%d\n", sc->nsects);
			mprintf("  Flags:	0x%.08x\n", sc->flags);
			*nofx = sc->nsects;
			ret = sizeof(*sc);
			free(sc);
			break;
		case LC_SYMTAB:
			symc = malloc(sizeof(*symc));
			memcpy(symc, ptr, sizeof(*symc));
			mprintf("  Symbol table offset:	%d bytes\n", symc->symoff);
			mprintf("  Symbol table entries:	%d\n", symc->nsyms);
			mprintf("  String table offset:	%d bytes\n", symc->stroff);
			mprintf("  String table size:	%d bytes\n", symc->strsize);
			ret = sizeof(*symc);
			free(symc);
			break;
		case LC_LOAD_DYLIB:
			dynamic = 1;
			dly = malloc(sizeof(*dly));
			memcpy(dly, ptr, sizeof(*dly));
			mprintf("  Name:			%s\n", ptr+dly->dylib.name.offset);
			timev = dly->dylib.timestamp;
			mprintf("  Timestamp:		%s", ctime(&timev));
			mprintf("  Current version:	0x%x\n", dly->dylib.current_version);
			mprintf("  Compat version:	0x%x\n", dly->dylib.compatibility_version);
			ret = sizeof(*dly);
			free(dly);
			break;
		case LC_LOAD_DYLINKER:
			dlnk = malloc(sizeof(*dlnk));
			memcpy(dlnk, ptr, sizeof(*dlnk));
			mprintf("  Name:		%s\n", ptr+dlnk->name.offset);
			ret = sizeof(*dlnk);
			free(dlnk);
			break;
		case LC_DYSYMTAB:
			dsym = malloc(sizeof(*dsym));
			memcpy(dsym, ptr, sizeof(*dsym));
			mprintf("  ilocalsym:		%d\n", dsym->ilocalsym);
			mprintf("  nlocalsym:		%d\n", dsym->nlocalsym);
			mprintf("  iextdefsym:		%d\n", dsym->iextdefsym);
			mprintf("  nextdefsym:		%d\n", dsym->nextdefsym);
			mprintf("  iundefsym:		%d\n", dsym->iundefsym);
			mprintf("  nundefsym:		%d\n", dsym->nundefsym);
			mprintf("  tocoff:		%d\n", dsym->tocoff);
			mprintf("  ntoc:			%d\n", dsym->ntoc);
			mprintf("  modtaboff:		%d\n", dsym->modtaboff);
			mprintf("  nmodtab:		%d\n", dsym->nmodtab);
			mprintf("  extrefsymoff:		%d\n", dsym->extrefsymoff);
			mprintf("  nextrefsyms:		%d\n", dsym->nextrefsyms);
			mprintf("  indirectsymoff:	%d\n", dsym->indirectsymoff);
			mprintf("  nindirectsyms:	%d\n", dsym->nindirectsyms);
			mprintf("  extreloff:		%d\n", dsym->extreloff);
			mprintf("  nextrel:		%d\n", dsym->nextrel);
			mprintf("  locreloff:		%d\n", dsym->locreloff);
			mprintf("  nlocrel:		%d\n", dsym->nlocrel);
			ret = sizeof(*dsym);
			free(dsym);
			break;
		case LC_TWOLEVEL_HINTS:
			two = malloc(sizeof(*two));
			memcpy(two, ptr, sizeof(*two));
			mprintf("  Offset:		%d\n", two->offset);
			mprintf("  No of 2level hints:	%d\n", two->nhints);			
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

void
display_text_section(char *buffer, int addr, int offset, int size)
{
	char *ptr;
	int i, j=0;
	
	if (offset <= 0)
		return;

	//printf("Addr: %x offset %x (%d) size %d\n", addr, offset, offset, size);
	
	ptr = buffer;
	ptr += offset;
			
	for (i=0; i<size; i++) {
		if (j == 0) printf("0x%.08x ", offset+i);
		j++;
		printf("%.02x ", (*ptr & 0xFF));
		if (j == 16) {
			printf("\n");
			j = 0;
		}
		ptr+=1;
	}
	printf("\n");
}

int
main (int argc, char **argv)
{
	struct stat sb;
	char *buffer, ch;
	int fd, len=1, offset = 0, ncmds = 0, flag = 0, ret;
	
	trigger = 0;
	dynamic = 0;
	
	if (argc < 2) {
		usage(argv[0]);
	}
	
	while ((ch = getopt(argc, argv, "uhct")) != -1) {
		switch (ch) {
		case 'u': flag |= 0x1; break;
		case 'h': flag |= 0x2; break;
		case 'c': flag |= 0x4; break;
		case 't': flag |= 0x8; break;
		default: usage(argv[0]); break;
		}
	}

	argc -= optind;
	argv += optind;
	
	if ((stat(argv[0], &sb)) < 0) 
		errx(1, "Cannot open %s", argv[1]);
	
	if ((fd = open(argv[0], O_RDONLY, 0)) < 0)
		errx(1, "Cannot open %s", argv[1]);
	
	if ((buffer = malloc(sb.st_size)) == NULL)
		errx(1, "Cannot allocate memory");
	
	len = read(fd, buffer, sb.st_size);
	
	switch (flag) {
		case 0x1:
			analyse_fat_header(buffer, &offset);
			break;
		case 0x2:
			trigger = 1;
			analyse_fat_header(buffer, &offset);
			trigger = 0;
			analyse_mo_header(buffer, &offset, &ncmds);
			break;
		case 0x3:
			analyse_fat_header(buffer, &offset);
			analyse_mo_header(buffer, &offset, &ncmds);
			break;
		case 0x4:
			analyse_fat_header(buffer, &offset);
			analyse_mo_header(buffer, &offset, &ncmds);
			analyse_load_command(buffer, offset, ncmds);
			break;
		case 0x8:
			trigger = 1;
			analyse_fat_header(buffer, &offset);
			analyse_mo_header(buffer, &offset, &ncmds);
			analyse_load_command(buffer, offset, ncmds);
			trigger = 0;
			display_text_section(buffer, text_addr, text_offset, text_size);
			break;
		default:
			trigger = 1;
			ret = analyse_fat_header(buffer, &offset);
			if (ret > 0)
				printf("Universal Binary for %d architectures, ", ret);
			ret = analyse_mo_header(buffer, &offset, &ncmds);
			if (ret > 0) {
				printf("Vaild ");
				trigger = 0;
				display_cpu_arch(ret);
				trigger = 1;				
				printf(" mach-o binary, ");
			} else {
				printf("No mach-o file\n");
				exit(1);
			}
			analyse_load_command(buffer, offset, ncmds);
			printf("%s linked ", dynamic ? "dynamically" : "statically");
			printf("\n");
			break;
	}

	close(fd);
	free(buffer);
	
	return (0);
}