#ifndef __ASM_DESC_DEFS_H__
#define __ASM_DESC_DEFS_H__

#include <linux/types.h>

struct desc_struct {
	union {
		struct {
			unsigned int a;
			unsigned int b;
		};
		struct {
			u16 limit0;
			u16 base0;
			unsigned base1: 8, type: 4, s: 1, dpl: 2, p: 1;
			unsigned limit: 4, avl: 1, l: 1, d: 1, g: 1, base2: 8;
		};
	};
} __attribute__((packed));

#endif /* ! __ASM_DESC_DEFS_H__ */
