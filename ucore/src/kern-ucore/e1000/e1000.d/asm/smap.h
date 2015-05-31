#ifndef __ASM_SMAP_H__
#define __ASM_SMAP_H__

#include <linux/stringify.h>
#include <asm/nops.h>
#include <asm/cpufeature.h>

#define __ASM_CLAC	.byte 0x0f,0x01,0xca
#define __ASM_STAC	.byte 0x0f,0x01,0xcb

#include <asm/alternative.h>

#define ASM_CLAC \
	ALTERNATIVE(ASM_NOP3, __stringify(__ASM_CLAC), X86_FEATURE_SMAP)
#define ASM_STAC \
	ALTERNATIVE(ASM_NOP3, __stringify(__ASM_STAC), X86_FEATURE_SMAP)

#endif /* ! __ASM_SMAP_H__ */
