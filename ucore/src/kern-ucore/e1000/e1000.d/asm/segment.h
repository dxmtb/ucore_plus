#ifndef __ASM_SEGMENT_H__
#define __ASM_SEGMENT_H__

#include <linux/const.h>

#include <asm/cache.h>

#define GDT_ENTRY_DEFAULT_USER_DS 5
#define GDT_ENTRY_DEFAULT_USER_CS 6

#define __USER32_DS	__USER_DS

#define GDT_ENTRY_PER_CPU 15	/* Abused to load per CPU data from limit */
#define __PER_CPU_SEG	(GDT_ENTRY_PER_CPU * 8 + 3)

#define __USER_DS	(GDT_ENTRY_DEFAULT_USER_DS*8+3)
#define __USER_CS	(GDT_ENTRY_DEFAULT_USER_CS*8+3)

#define NUM_EXCEPTION_VECTORS 32

#define GDT_ENTRY_TLS_ENTRIES 3

#endif /* ! __ASM_SEGMENT_H__ */
