#ifndef __UAPI_LINUX_ELF_H__
#define __UAPI_LINUX_ELF_H__

#include <linux/types.h>
#include <linux/elf-em.h>

typedef __u64	Elf64_Addr;
typedef __u16	Elf64_Half;

typedef __u32	Elf64_Word;
typedef __u64	Elf64_Xword;

typedef struct elf64_sym {
  Elf64_Word st_name;		/* Symbol name, index in string tbl */
  unsigned char	st_info;	/* Type and binding attributes */
  unsigned char	st_other;	/* No defined meaning, 0 */
  Elf64_Half st_shndx;		/* Associated section index */
  Elf64_Addr st_value;		/* Value of the symbol */
  Elf64_Xword st_size;		/* Associated symbol size */
} Elf64_Sym;

#define EI_NIDENT	16

#define	ELFCLASS32	1
#define	ELFCLASS64	2

#endif /* ! __UAPI_LINUX_ELF_H__ */
