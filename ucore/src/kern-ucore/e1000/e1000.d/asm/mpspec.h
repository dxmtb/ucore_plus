#ifndef __ASM_MPSPEC_H__
#define __ASM_MPSPEC_H__

#include <linux/init.h>

#include <asm/mpspec_def.h>
#include <asm/x86_init.h>
#include <asm/apicdef.h>

#define MAX_MP_BUSSES		256

#define MAX_IRQ_SOURCES		(MAX_MP_BUSSES * 4)

extern int smp_found_config;

struct device;

#define PHYSID_ARRAY_SIZE	BITS_TO_LONGS(MAX_LOCAL_APIC)

struct physid_mask {
	unsigned long mask[PHYSID_ARRAY_SIZE];
};

typedef struct physid_mask physid_mask_t;

#define physid_set(physid, map)			set_bit(physid, (map).mask)

#define physid_isset(physid, map)		test_bit(physid, (map).mask)

#define physids_clear(map)					\
	bitmap_zero((map).mask, MAX_LOCAL_APIC)

extern physid_mask_t phys_cpu_present_map;

#endif /* ! __ASM_MPSPEC_H__ */
