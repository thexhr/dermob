/*
 * Copyright (c) 1999-2010 Apple Inc.  All Rights Reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */


#ifndef __MACH_H
#define __MACH_H

#define MH_OBJECT       0x1
#define MH_EXECUTE      0x2 
#define MH_FVMLIB       0x3 
#define MH_CORE         0x4 
#define MH_PRELOAD      0x5 
#define MH_DYLIB        0x6 
#define MH_DYLINKER     0x7 
#define MH_BUNDLE       0x8 
#define MH_DYLIB_STUB   0x9 
#define MH_NOUNDEFS     0x1 
#define MH_INCRLINK     0x2 
#define MH_DYLDLINK     0x4 
#define MH_BINDATLOAD   0x8 
#define MH_PREBOUND     0x10
#define MH_SPLIT_SEGS   0x20
#define MH_LAZY_INIT    0x40
#define MH_TWOLEVEL     0x80
#define MH_FORCE_FLAT   0x100           
#define MH_NOMULTIDEFS  0x200           
#define MH_NOFIXPREBINDING 0x400        
#define MH_PREBINDABLE  0x800           
#define MH_ALLMODSBOUND 0x1000          
#define MH_SUBSECTIONS_VIA_SYMBOLS 0x2000
#define MH_CANONICAL    0x4000
#define MH_WEAK_DEFINES 0x8000
#define MH_BINDS_TO_WEAK 0x10000
#define MH_ALLOW_STACK_EXECUTION 0x20000

#define LC_REQ_DYLD 0x80000000

#define LC_SEGMENT      0x1
#define LC_SYMTAB       0x2
#define LC_SYMSEG       0x3
#define LC_THREAD       0x4
#define LC_UNIXTHREAD   0x5
#define LC_LOADFVMLIB   0x6
#define LC_IDFVMLIB     0x7
#define LC_IDENT        0x8
#define LC_FVMFILE      0x9
#define LC_PREPAGE      0xa
#define LC_DYSYMTAB     0xb
#define LC_LOAD_DYLIB   0xc
#define LC_ID_DYLIB     0xd
#define LC_LOAD_DYLINKER 0xe
#define LC_ID_DYLINKER  0xf
#define LC_PREBOUND_DYLIB 0x10
#define LC_ROUTINES     0x11   
#define LC_SUB_FRAMEWORK 0x12  
#define LC_SUB_UMBRELLA 0x13   
#define LC_SUB_CLIENT   0x14   
#define LC_SUB_LIBRARY  0x15   
#define LC_TWOLEVEL_HINTS 0x16 
#define LC_PREBIND_CKSUM  0x17 

#define LC_LOAD_WEAK_DYLIB (0x18 | LC_REQ_DYLD)

#define SG_HIGHVM       0x1
#define SG_FVMLIB       0x2
#define SG_NORELOC      0x4
#define SG_PROTECTED_VERSION_1  0x8

#define SECTION_TYPE             0x000000ff
#define SECTION_ATTRIBUTES       0xffffff00

#define S_REGULAR               0x0
#define S_ZEROFILL              0x1
#define S_CSTRING_LITERALS      0x2
#define S_4BYTE_LITERALS        0x3
#define S_8BYTE_LITERALS        0x4
#define S_LITERAL_POINTERS      0x5

#define S_NON_LAZY_SYMBOL_POINTERS      0x6
#define S_LAZY_SYMBOL_POINTERS          0x7
#define S_SYMBOL_STUBS                  0x8
#define S_MOD_INIT_FUNC_POINTERS        0x9
#define S_MOD_TERM_FUNC_POINTERS        0xa
#define S_COALESCED                     0xb
#define S_GB_ZEROFILL                   0xc
#define S_INTERPOSING                   0xd

#define SECTION_ATTRIBUTES_USR   0xff000000
#define S_ATTR_PURE_INSTRUCTIONS 0x80000000
#define S_ATTR_NO_TOC            0x40000000
#define S_ATTR_STRIP_STATIC_SYMS 0x20000000
#define S_ATTR_NO_DEAD_STRIP     0x10000000
#define S_ATTR_LIVE_SUPPORT      0x08000000
#define S_ATTR_SELF_MODIFYING_CODE 0x04000000
#define SECTION_ATTRIBUTES_SYS   0x00ffff00
#define S_ATTR_SOME_INSTRUCTIONS 0x00000400
#define S_ATTR_EXT_RELOC         0x00000200
#define S_ATTR_LOC_RELOC         0x00000100                                           

#define SEG_PAGEZERO    "__PAGEZERO"
#define SEG_TEXT        "__TEXT"
#define SECT_TEXT       "__text"
#define SECT_FVMLIB_INIT0 "__fvmlib_init0"
#define SECT_FVMLIB_INIT1 "__fvmlib_init1"
#define SEG_DATA        "__DATA"
#define SECT_DATA       "__data"
#define SECT_BSS        "__bss"
#define SECT_COMMON     "__common"
#define SEG_OBJC        "__OBJC"
#define SECT_OBJC_SYMBOLS "__symbol_table"
#define SECT_OBJC_MODULES "__module_info"
#define SECT_OBJC_STRINGS "__selector_strs"
#define SECT_OBJC_REFS "__selector_refs"
#define SEG_ICON         "__ICON"
#define SECT_ICON_HEADER "__header"
#define SECT_ICON_TIFF   "__tiff"
#define SEG_LINKEDIT    "__LINKEDIT"
#define SEG_UNIXSTACK   "__UNIXSTACK"
#define SEG_IMPORT      "__IMPORT"

/*
 * Capability bits used in the definition of cpu_type.
 */
#define CPU_ARCH_MASK   0xff000000              /* mask for architecture bits */
#define CPU_ARCH_ABI64  0x01000000              /* 64 bit ABI */

#define CPU_TYPE_ANY		((cpu_type_t) -1)

#define CPU_TYPE_VAX		((cpu_type_t) 1)
/* skip				((cpu_type_t) 2)	*/
/* skip				((cpu_type_t) 3)	*/
/* skip				((cpu_type_t) 4)	*/
/* skip				((cpu_type_t) 5)	*/
#define	CPU_TYPE_MC680x0	((cpu_type_t) 6)
#define CPU_TYPE_X86		((cpu_type_t) 7)
#define CPU_TYPE_I386		CPU_TYPE_X86		/* compatibility */
#define	CPU_TYPE_X86_64		(CPU_TYPE_X86 | CPU_ARCH_ABI64)

/* skip CPU_TYPE_MIPS		((cpu_type_t) 8)	*/
/* skip 			((cpu_type_t) 9)	*/
#define CPU_TYPE_MC98000	((cpu_type_t) 10)
#define CPU_TYPE_HPPA           ((cpu_type_t) 11)
#define CPU_TYPE_ARM		((cpu_type_t) 12)
#define CPU_TYPE_MC88000	((cpu_type_t) 13)
#define CPU_TYPE_SPARC		((cpu_type_t) 14)
#define CPU_TYPE_I860		((cpu_type_t) 15)
/* skip	CPU_TYPE_ALPHA		((cpu_type_t) 16)	*/
/* skip				((cpu_type_t) 17)	*/
#define CPU_TYPE_POWERPC		((cpu_type_t) 18)
#define CPU_TYPE_POWERPC64		(CPU_TYPE_POWERPC | CPU_ARCH_ABI64)

/*
 *	Machine subtypes (these are defined here, instead of in a machine
 *	dependent directory, so that any program can get all definitions
 *	regardless of where is it compiled).
 */

/*
 * Capability bits used in the definition of cpu_subtype.
 */
#define CPU_SUBTYPE_MASK	0xff000000	/* mask for feature flags */
#define CPU_SUBTYPE_LIB64	0x80000000	/* 64 bit libraries */


/*
 *	Object files that are hand-crafted to run on any
 *	implementation of an architecture are tagged with
 *	CPU_SUBTYPE_MULTIPLE.  This functions essentially the same as
 *	the "ALL" subtype of an architecture except that it allows us
 *	to easily find object files that may need to be modified
 *	whenever a new implementation of an architecture comes out.
 *
 *	It is the responsibility of the implementor to make sure the
 *	software handles unsupported implementations elegantly.
 */
#define	CPU_SUBTYPE_MULTIPLE		((cpu_subtype_t) -1)
#define CPU_SUBTYPE_LITTLE_ENDIAN	((cpu_subtype_t) 0)
#define CPU_SUBTYPE_BIG_ENDIAN		((cpu_subtype_t) 1)

/*
 *     Machine threadtypes.
 *     This is none - not defined - for most machine types/subtypes.
 */
#define CPU_THREADTYPE_NONE		((cpu_threadtype_t) 0)

/*
 *	VAX subtypes (these do *not* necessary conform to the actual cpu
 *	ID assigned by DEC available via the SID register).
 */

#define	CPU_SUBTYPE_VAX_ALL	((cpu_subtype_t) 0) 
#define CPU_SUBTYPE_VAX780	((cpu_subtype_t) 1)
#define CPU_SUBTYPE_VAX785	((cpu_subtype_t) 2)
#define CPU_SUBTYPE_VAX750	((cpu_subtype_t) 3)
#define CPU_SUBTYPE_VAX730	((cpu_subtype_t) 4)
#define CPU_SUBTYPE_UVAXI	((cpu_subtype_t) 5)
#define CPU_SUBTYPE_UVAXII	((cpu_subtype_t) 6)
#define CPU_SUBTYPE_VAX8200	((cpu_subtype_t) 7)
#define CPU_SUBTYPE_VAX8500	((cpu_subtype_t) 8)
#define CPU_SUBTYPE_VAX8600	((cpu_subtype_t) 9)
#define CPU_SUBTYPE_VAX8650	((cpu_subtype_t) 10)
#define CPU_SUBTYPE_VAX8800	((cpu_subtype_t) 11)
#define CPU_SUBTYPE_UVAXIII	((cpu_subtype_t) 12)

/*
 * 	680x0 subtypes
 *
 * The subtype definitions here are unusual for historical reasons.
 * NeXT used to consider 68030 code as generic 68000 code.  For
 * backwards compatability:
 * 
 *	CPU_SUBTYPE_MC68030 symbol has been preserved for source code
 *	compatability.
 *
 *	CPU_SUBTYPE_MC680x0_ALL has been defined to be the same
 *	subtype as CPU_SUBTYPE_MC68030 for binary comatability.
 *
 *	CPU_SUBTYPE_MC68030_ONLY has been added to allow new object
 *	files to be tagged as containing 68030-specific instructions.
 */

#define	CPU_SUBTYPE_MC680x0_ALL		((cpu_subtype_t) 1)
#define CPU_SUBTYPE_MC68030		((cpu_subtype_t) 1) /* compat */
#define CPU_SUBTYPE_MC68040		((cpu_subtype_t) 2) 
#define	CPU_SUBTYPE_MC68030_ONLY	((cpu_subtype_t) 3)

/*
 *	I386 subtypes
 */

#define CPU_SUBTYPE_INTEL(f, m)	((cpu_subtype_t) (f) + ((m) << 4))

#define	CPU_SUBTYPE_I386_ALL			CPU_SUBTYPE_INTEL(3, 0)
#define CPU_SUBTYPE_386					CPU_SUBTYPE_INTEL(3, 0)
#define CPU_SUBTYPE_486					CPU_SUBTYPE_INTEL(4, 0)
#define CPU_SUBTYPE_486SX				CPU_SUBTYPE_INTEL(4, 8)	// 8 << 4 = 128
#define CPU_SUBTYPE_586					CPU_SUBTYPE_INTEL(5, 0)
#define CPU_SUBTYPE_PENT	CPU_SUBTYPE_INTEL(5, 0)
#define CPU_SUBTYPE_PENTPRO	CPU_SUBTYPE_INTEL(6, 1)
#define CPU_SUBTYPE_PENTII_M3	CPU_SUBTYPE_INTEL(6, 3)
#define CPU_SUBTYPE_PENTII_M5	CPU_SUBTYPE_INTEL(6, 5)
#define CPU_SUBTYPE_CELERON				CPU_SUBTYPE_INTEL(7, 6)
#define CPU_SUBTYPE_CELERON_MOBILE		CPU_SUBTYPE_INTEL(7, 7)
#define CPU_SUBTYPE_PENTIUM_3			CPU_SUBTYPE_INTEL(8, 0)
#define CPU_SUBTYPE_PENTIUM_3_M			CPU_SUBTYPE_INTEL(8, 1)
#define CPU_SUBTYPE_PENTIUM_3_XEON		CPU_SUBTYPE_INTEL(8, 2)
#define CPU_SUBTYPE_PENTIUM_M			CPU_SUBTYPE_INTEL(9, 0)
#define CPU_SUBTYPE_PENTIUM_4			CPU_SUBTYPE_INTEL(10, 0)
#define CPU_SUBTYPE_PENTIUM_4_M			CPU_SUBTYPE_INTEL(10, 1)
#define CPU_SUBTYPE_ITANIUM				CPU_SUBTYPE_INTEL(11, 0)
#define CPU_SUBTYPE_ITANIUM_2			CPU_SUBTYPE_INTEL(11, 1)
#define CPU_SUBTYPE_XEON				CPU_SUBTYPE_INTEL(12, 0)
#define CPU_SUBTYPE_XEON_MP				CPU_SUBTYPE_INTEL(12, 1)

#define CPU_SUBTYPE_INTEL_FAMILY(x)	((x) & 15)
#define CPU_SUBTYPE_INTEL_FAMILY_MAX	15

#define CPU_SUBTYPE_INTEL_MODEL(x)	((x) >> 4)
#define CPU_SUBTYPE_INTEL_MODEL_ALL	0

/*
 *	X86 subtypes.
 */

#define CPU_SUBTYPE_X86_ALL		((cpu_subtype_t)3)
#define CPU_SUBTYPE_X86_64_ALL		((cpu_subtype_t)3)
#define CPU_SUBTYPE_X86_ARCH1		((cpu_subtype_t)4)


#define CPU_THREADTYPE_INTEL_HTT	((cpu_threadtype_t) 1)

/*
 *	Mips subtypes.
 */

#define	CPU_SUBTYPE_MIPS_ALL	((cpu_subtype_t) 0)
#define CPU_SUBTYPE_MIPS_R2300	((cpu_subtype_t) 1)
#define CPU_SUBTYPE_MIPS_R2600	((cpu_subtype_t) 2)
#define CPU_SUBTYPE_MIPS_R2800	((cpu_subtype_t) 3)
#define CPU_SUBTYPE_MIPS_R2000a	((cpu_subtype_t) 4)	/* pmax */
#define CPU_SUBTYPE_MIPS_R2000	((cpu_subtype_t) 5)
#define CPU_SUBTYPE_MIPS_R3000a	((cpu_subtype_t) 6)	/* 3max */
#define CPU_SUBTYPE_MIPS_R3000	((cpu_subtype_t) 7)

/*
 *	MC98000 (PowerPC) subtypes
 */
#define	CPU_SUBTYPE_MC98000_ALL	((cpu_subtype_t) 0)
#define CPU_SUBTYPE_MC98601	((cpu_subtype_t) 1)

/*
 *	HPPA subtypes for Hewlett-Packard HP-PA family of
 *	risc processors. Port by NeXT to 700 series. 
 */

#define	CPU_SUBTYPE_HPPA_ALL		((cpu_subtype_t) 0)
#define CPU_SUBTYPE_HPPA_7100		((cpu_subtype_t) 0) /* compat */
#define CPU_SUBTYPE_HPPA_7100LC		((cpu_subtype_t) 1)

/*
 *	MC88000 subtypes.
 */
#define	CPU_SUBTYPE_MC88000_ALL	((cpu_subtype_t) 0)
#define CPU_SUBTYPE_MC88100	((cpu_subtype_t) 1)
#define CPU_SUBTYPE_MC88110	((cpu_subtype_t) 2)

/*
 *	SPARC subtypes
 */
#define	CPU_SUBTYPE_SPARC_ALL		((cpu_subtype_t) 0)

/*
 *	I860 subtypes
 */
#define CPU_SUBTYPE_I860_ALL	((cpu_subtype_t) 0)
#define CPU_SUBTYPE_I860_860	((cpu_subtype_t) 1)

/*
 *	PowerPC subtypes
 */
#define CPU_SUBTYPE_POWERPC_ALL		((cpu_subtype_t) 0)
#define CPU_SUBTYPE_POWERPC_601		((cpu_subtype_t) 1)
#define CPU_SUBTYPE_POWERPC_602		((cpu_subtype_t) 2)
#define CPU_SUBTYPE_POWERPC_603		((cpu_subtype_t) 3)
#define CPU_SUBTYPE_POWERPC_603e	((cpu_subtype_t) 4)
#define CPU_SUBTYPE_POWERPC_603ev	((cpu_subtype_t) 5)
#define CPU_SUBTYPE_POWERPC_604		((cpu_subtype_t) 6)
#define CPU_SUBTYPE_POWERPC_604e	((cpu_subtype_t) 7)
#define CPU_SUBTYPE_POWERPC_620		((cpu_subtype_t) 8)
#define CPU_SUBTYPE_POWERPC_750		((cpu_subtype_t) 9)
#define CPU_SUBTYPE_POWERPC_7400	((cpu_subtype_t) 10)
#define CPU_SUBTYPE_POWERPC_7450	((cpu_subtype_t) 11)
#define CPU_SUBTYPE_POWERPC_970		((cpu_subtype_t) 100)

/*
 *	ARM subtypes
 */
#define CPU_SUBTYPE_ARM_ALL             ((cpu_subtype_t) 0)
#define CPU_SUBTYPE_ARM_V4T             ((cpu_subtype_t) 5)
#define CPU_SUBTYPE_ARM_V6              ((cpu_subtype_t) 6)
#define CPU_SUBTYPE_ARM_V5TEJ           ((cpu_subtype_t) 7)
#define CPU_SUBTYPE_ARM_XSCALE		((cpu_subtype_t) 8)
#define CPU_SUBTYPE_ARM_V7		((cpu_subtype_t) 9)


typedef int vm_prot_t;
typedef int cpu_type_t;
typedef int cpu_subtype_t;
typedef int cpu_threadtype_t;

#define FAT_MAGIC	0xcafebabe
#define FAT_CIGAM	0xbebafeca

#define MH_MAGIC        0xfeedface
#define MH_CIGAM        0xcefaedfe
#define MH_MAGIC_64 	0xfeedfacf /* the 64-bit mach magic number */
#define MH_CIGAM_64 	0xcffaedfe /* NXSwapInt(MH_MAGIC_64) */

struct fat_header {
	uint32_t	magic;
	uint32_t	nfat_arch;
};

struct fat_arch {
	cpu_type_t	cputype;
	cpu_subtype_t	cpusubtype;
	uint32_t	offset;
	uint32_t	size;
	uint32_t	align;
};

struct mach_header_magic {
	uint32_t        magic;
};

struct mach_header {
	uint32_t        magic;
	cpu_type_t      cputype;
	cpu_subtype_t   cpusubtype;
	uint32_t        filetype;
	uint32_t        ncmds;
	uint32_t        sizeofcmds;
	uint32_t        flags;
};

struct mach_header_64 {
        uint32_t        magic;          /* mach magic number identifier */
        cpu_type_t      cputype;        /* cpu specifier */
        cpu_subtype_t   cpusubtype;     /* machine specifier */
        uint32_t        filetype;       /* type of file */
        uint32_t        ncmds;          /* number of load commands */
        uint32_t        sizeofcmds;     /* the size of all the load commands */
        uint32_t        flags;          /* flags */
        uint32_t        reserved;       /* reserved */
};

struct load_command {
	uint32_t cmd;
	uint32_t cmdsize;
};

struct segment_command {
	uint32_t        cmd;
	uint32_t        cmdsize;
	char            segname[16];
	uint32_t        vmaddr;
	uint32_t        vmsize;
	uint32_t        fileoff;
	uint32_t        filesize;
	vm_prot_t       maxprot;
	vm_prot_t       initprot;
	uint32_t        nsects;
	uint32_t        flags;
};

struct segment_command_64 { /* for 64-bit architectures */
        uint32_t        cmd;            /* LC_SEGMENT_64 */
        uint32_t        cmdsize;        /* includes sizeof section_64 structs */
        char            segname[16];    /* segment name */
        uint64_t        vmaddr;         /* memory address of this segment */
        uint64_t        vmsize;         /* memory size of this segment */
        uint64_t        fileoff;        /* file offset of this segment */
        uint64_t        filesize;       /* amount to map from the file */
        vm_prot_t       maxprot;        /* maximum VM protection */
        vm_prot_t       initprot;       /* initial VM protection */
        uint32_t        nsects;         /* number of sections in segment */
        uint32_t        flags;          /* flags */
};

struct section {
	char            sectname[16];
	char            segname[16];
	uint32_t        addr;
	uint32_t        size;
	uint32_t        offset;
	uint32_t        align;
	uint32_t        reloff;
	uint32_t        nreloc;
	uint32_t        flags;
	uint32_t        reserved1;
	uint32_t        reserved2;
};

struct section_64 { /* for 64-bit architectures */
        char            sectname[16];   /* name of this section */
        char            segname[16];    /* segment this section goes in */
        uint64_t        addr;           /* memory address of this section */
        uint64_t        size;           /* size in bytes of this section */
        uint32_t        offset;         /* file offset of this section */
        uint32_t        align;          /* section alignment (power of 2) */
        uint32_t        reloff;         /* file offset of relocation entries */
        uint32_t        nreloc;         /* number of relocation entries */
        uint32_t        flags;          /* flags (section type and attributes)*/
        uint32_t        reserved1;      /* reserved (for offset or index) */
        uint32_t        reserved2;      /* reserved (for count or sizeof) */
        uint32_t        reserved3;      /* reserved */
};

union lc_str {
	uint32_t        offset;
#ifndef __LP64__
	char            *ptr;
#endif 
};

struct symtab_command {
	uint32_t	cmd;
	uint32_t	cmdsize;
	uint32_t	symoff;
	uint32_t	nsyms;
	uint32_t	stroff;
	uint32_t	strsize;
};

struct dylib {
	union lc_str  name;
	uint32_t timestamp;
	uint32_t current_version;
	uint32_t compatibility_version;
};

struct dylib_command {
	uint32_t        cmd;
	uint32_t        cmdsize;
	struct dylib    dylib;
};

struct dylinker_command {
	uint32_t        cmd;
	uint32_t        cmdsize;
	union lc_str    name;
};

struct dysymtab_command {
	uint32_t cmd;
	uint32_t cmdsize;
	uint32_t ilocalsym;
	uint32_t nlocalsym;
	uint32_t iextdefsym;
	uint32_t nextdefsym;
	uint32_t iundefsym;
	uint32_t nundefsym;
	uint32_t tocoff;
	uint32_t ntoc;
	uint32_t modtaboff;
	uint32_t nmodtab;
	uint32_t extrefsymoff;
	uint32_t nextrefsyms;
	uint32_t indirectsymoff;
	uint32_t nindirectsyms;
	uint32_t extreloff;
	uint32_t nextrel;
	uint32_t locreloff;
	uint32_t nlocrel;
};

struct dylib_table_of_contents {
	uint32_t symbol_index;
	uint32_t module_index;
};	

struct dylib_module {
	uint32_t module_name;
	uint32_t iextdefsym;
	uint32_t nextdefsym;
	uint32_t irefsym;
	uint32_t nrefsym;
	uint32_t ilocalsym;
	uint32_t nlocalsym;
	uint32_t iextrel;
	uint32_t nextrel;
	uint32_t iinit_iterm;
	uint32_t ninit_nterm;
	uint32_t objc_module_info_addr;
	uint32_t objc_module_info_size;
};

struct twolevel_hints_command {
	uint32_t cmd;
	uint32_t cmdsize;
	uint32_t offset;
	uint32_t nhints;
};


#endif
