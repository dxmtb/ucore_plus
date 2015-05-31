#ifndef __ASM_APICDEF_H__
#define __ASM_APICDEF_H__

#define	APIC_ID		0x20

#define	APIC_LVR	0x30

#define		GET_APIC_VERSION(x)	((x) & 0xFFu)

#define		APIC_XAPIC(x)		((x) >= 0x14)

#define	APIC_EOI	0xB0
#define		APIC_EOI_ACK		0x0 /* Docs say 0 for future compat. */

#define		APIC_ALL_CPUS		0xFFu

#define APIC_BASE (fix_to_virt(FIX_APIC_BASE))

# define MAX_IO_APICS 128
# define MAX_LOCAL_APIC 32768

/*
 * the local APIC register structure, memory mapped. Not terribly well
 * tested, but we might eventually use this one in the future - the
 * problem why we cannot use it right now is the P5 APIC, it has an
 * errata which cannot take 8-bit reads and writes, only 32-bit ones ...
 */
#define u32 unsigned int

#undef u32

 #define BAD_APICID 0xFFFFu

#endif /* ! __ASM_APICDEF_H__ */
