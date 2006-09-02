#ifndef __DEFS_H
#define __DEFS_H

int display_fat_header(struct list *lst, char *, int *);
int display_mo_header(struct list *lst, char *, int *, int *);
void display_load_commands(struct list *lst, char *, int *, int );
void print_section(struct section *);
void print_load_command(struct load_command *);
void print_mo_header(struct mach_header *);
void print_fat_header(struct fat_header *);
void print_fat_arch(struct fat_arch *);
void print_lc_towlevel_hints(struct twolevel_hints_command *two);
void display_cmd_name(int);
void display_cpu_arch(int);
void print_lc_towlevel_hints(struct twolevel_hints_command *two);
void print_lc_dysymtab(struct dysymtab_command *dsym);
void print_lc_load_dylib(struct dylib_command *dly);
void print_lc_segment(struct segment_command *sc);
void mprintf(const char *fmt, ...);
int get_cpu_information();
int get_bo_information();
unsigned int swapi(unsigned int);

int analyse_fat_header(char *, int *, struct fat_header*);
int analyse_fat_arch(char *, int *, struct fat_arch *);
int analyse_mo_header(char *, int *, struct mach_header *);
void analyse_load_command(char *, int *, struct load_command *);
void examine_section(char *, int *, struct section *);
int examine_segmet(char *, int *, int , int , int *);
void display_buffer(char *, int , int , int );

#endif
