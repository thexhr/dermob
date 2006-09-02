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

/* $Id: dermob-cli.c,v 1.11 2006/09/02 01:29:51 matthias Exp $ */

#include "dermob.h"
#include "mach.h"
#include "defs.h"
#include "list.h"

void
usage(const char *file)
{
	printf("Usage: 	%s [-cdhstux] <binary>\n", file);
	printf("	-c:  Display complete header\n");
	printf("	-d:  Display __DATA,__data section\n");	
	printf("	-h:  Display mach-o header\n");
	printf("	-s:  Display __TEXT,__csting section\n");
	printf("	-t:  Display __TEXT,__text section\n");	
	printf("	-u:  Display universal header\n");
	printf("	-x:  Display hexdump\n");
	exit(1);
}

int
main (int argc, char **argv)
{
	struct stat sb;
	char *buffer, ch;
	int fd, len=1, offset = 0, ncmds = 0, flag = 0, ret;
	
	trigger = 0;
	dynamic = 0;
	dyn_display = 0;
	
	if (argc < 2) {
		usage(argv[0]);
	}
	
	while ((ch = getopt(argc, argv, "uhctxds")) != -1) {
		switch (ch) {
		case 'u': flag |= 0x1; break;
		case 'h': flag |= 0x2; break;
		case 'c': flag |= 0x4; break;
		case 't': flag |= 0x8; break;
		case 'x': flag |= 0x16; break;
		case 'd': flag |= 0x32; break;
		case 's': flag |= 0x64; break;
		default: usage(argv[0]); break;
		}
	}

	argc -= optind;
	argv += optind;
	
	if ((stat(argv[0], &sb)) < 0) 
		errx(1, "Cannot open %s", argv[0]);
	
	if ((fd = open(argv[0], O_RDONLY, 0)) < 0)
		errx(1, "Cannot open %s", argv[0]);
	
	if ((buffer = malloc(sb.st_size)) == NULL)
		errx(1, "Cannot allocate memory");
	
	size = sb.st_size;
	len = read(fd, buffer, size);
	
	cpu = get_cpu_information();
	bo_a = get_bo_information();
	
	lst = list_create_list();
	
	switch (flag) {
		// -u
		case 0x1:
			display_fat_header(lst, buffer, &offset);
			list_traverse_list(lst);
			break;
		// -h
		case 0x2:
			display_fat_header(lst, buffer, &offset);
			display_mo_header(lst, buffer, &offset, &ncmds);
			list_traverse_list(lst);
			break;
		// -uh
		case 0x3:
			display_fat_header(lst, buffer, &offset);
			display_mo_header(lst, buffer, &offset, &ncmds);
			list_traverse_list(lst);
			break;
		// -c
		case 0x4:
			display_fat_header(lst, buffer, &offset);
			display_mo_header(lst, buffer, &offset, &ncmds);
			display_load_commands(lst, buffer, &offset, ncmds);
			list_traverse_list(lst);
			break;
		// -t
		case 0x8:
			display_fat_header(lst, buffer, &offset);
			display_mo_header(lst, buffer, &offset, &ncmds);
			display_load_commands(lst, buffer, &offset, ncmds);
			display_buffer(buffer, text_addr, text_offset, text_size);
			break;
		// -x
		case 0x16:
			display_buffer(buffer, 0, 0, len);
			break;
		// -d
		case 0x32:
			display_fat_header(lst, buffer, &offset);
			display_mo_header(lst, buffer, &offset, &ncmds);
			display_load_commands(lst, buffer, &offset, ncmds);
			display_buffer(buffer, data_addr, data_offset, data_size);
			break;
		// -s
		case 0x64:
			display_fat_header(lst, buffer, &offset);
			display_mo_header(lst, buffer, &offset, &ncmds);
			display_load_commands(lst, buffer, &offset, ncmds);
			display_buffer(buffer, cs_addr, cs_offset, cs_size);
			break;
		default:
			ret = display_fat_header(lst, buffer, &offset);
			if (ret > 0)
				printf("- Universal Binary for %d architectures\n", ret);
			ret = display_mo_header(lst, buffer, &offset, &ncmds);
			if (ret > 0)
				printf("- Vaild mach-o binary\n");
			else {
				printf("No mach-o file\n");
				exit(1);
			}
			dyn_display = 1;
			display_load_commands(lst, buffer, &offset, ncmds);
			printf("%s", dynamic ? "" : "- Statically linked\n");
			break;
	}

	list_free_list(lst);
	
	close(fd);
	free(buffer);
	
	return (0);
}
