#include <stdint.h>
#include <assert.h>
#include <cstdio>
#include "elf.h"

#define ELF_MAGIC   0x464C457FU         // "\x7FELF" in little endian

/* file header */
struct elfhdr {
    uint32_t e_magic;     // must equal ELF_MAGIC
    uint8_t e_elf[12];
    uint16_t e_type;      // 1=relocatable, 2=executable, 3=shared object, 4=core image
    uint16_t e_machine;   // 3=x86, 4=68K, etc.
    uint32_t e_version;   // file version, always 1
    uint32_t e_entry;     // entry point if executable
    uint32_t e_phoff;     // file position of program header or 0
    uint32_t e_shoff;     // file position of section header or 0
    uint32_t e_flags;     // architecture-specific flags, usually 0
    uint16_t e_ehsize;    // size of this elf header
    uint16_t e_phentsize; // size of an entry in program header
    uint16_t e_phnum;     // number of entries in program header or 0
    uint16_t e_shentsize; // size of an entry in section header
    uint16_t e_shnum;     // number of entries in section header or 0
    uint16_t e_shstrndx;  // section number that contains section name strings
};

/* program section header */
struct proghdr {
    uint32_t p_type;   // loadable code or data, dynamic linking info,etc.
    uint32_t p_offset; // file offset of segment
    uint32_t p_va;     // virtual address to map segment
    uint32_t p_pa;     // physical address, not used
    uint32_t p_filesz; // size of segment in file
    uint32_t p_memsz;  // size of segment in memory (bigger if contains bss）
    uint32_t p_flags;  // read/write/execute bits
    uint32_t p_align;  // required alignment, invariably hardware page size
};

static unsigned long load_elf_image_phdr(unsigned long addr)
{
	Elf32_Ehdr *ehdr;		/* Elf header structure pointer     */
	Elf32_Phdr *phdr;		/* Program header structure pointer */
	int i;

	ehdr = (Elf32_Ehdr *) addr;
	phdr = (Elf32_Phdr *) (addr + ehdr->e_phoff);

	/* Load each program header */
	for (i = 0; i < ehdr->e_phnum; ++i) {
		void *dst = (void *)(uintptr_t) phdr->p_paddr;
		void *src = (void*)((char *) addr + phdr->p_offset);
		printf("Loading phdr %i to 0x%p (%i bytes)\n", i, dst, phdr->p_filesz);
		if (phdr->p_filesz)
			printf("Memcpy %x %x %x\n", dst, src, phdr->p_filesz);
		if (phdr->p_filesz != phdr->p_memsz)
			printf("Memset %x %x %x\n", (char*)dst + phdr->p_filesz, 0x00,
				phdr->p_memsz - phdr->p_filesz);
		++phdr;
	}

	printf("entry %x\n", ehdr->e_entry);
}

int main(int argc, char *argv[]) {
    assert(argc == 3);
    FILE *f = fopen(argv[1], "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = (char*) malloc(fsize);
    fread(buf, fsize, 1, f);
    fclose(f);


    struct elfhdr *ELFHDR = (struct elfhdr *) buf;

	assert (ELFHDR->e_magic == ELF_MAGIC);

	struct proghdr *ph, *eph;

	// load each program segment (ignores ph flags)
	ph = (struct proghdr *)((uintptr_t) ELFHDR + ELFHDR->e_phoff);
	eph = ph + ELFHDR->e_phnum;
	for (; ph < eph; ph++) {
        ph->p_pa &= 0xFFFFFF;
        printf("ph pa: %x\n", ph->p_pa);
	}

	ELFHDR->e_entry &= 0xFFFFFF;
    printf("entry: %x\n", ELFHDR->e_entry);

    f = fopen(argv[2], "wb");
    fwrite(buf, fsize, 1, f);
    fclose(f);

    load_elf_image_phdr((unsigned long)buf);

    return 0;
}

