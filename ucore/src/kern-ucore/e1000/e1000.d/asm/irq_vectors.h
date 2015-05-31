#ifndef __ASM_IRQ_VECTORS_H__
#define __ASM_IRQ_VECTORS_H__

#include <linux/threads.h>

/*
 * IDT vectors usable for external interrupt sources start at 0x20.
 * (0x80 is the syscall vector, 0x30-0x3f are for ISA)
 */
#define FIRST_EXTERNAL_VECTOR		0x20

#define SPURIOUS_APIC_VECTOR		0xff

#define NR_VECTORS			 256

#define	FIRST_VM86_IRQ			   3
#define LAST_VM86_IRQ			  15

#define NR_IRQS_LEGACY			  16

#endif /* ! __ASM_IRQ_VECTORS_H__ */
