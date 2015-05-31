#ifndef __ASM_SPINLOCK_TYPES_H__
#define __ASM_SPINLOCK_TYPES_H__

#include <linux/types.h>

typedef u8  __ticket_t;
typedef u16 __ticketpair_t;

#define TICKET_SHIFT	(sizeof(__ticket_t) * 8)

typedef struct arch_spinlock {
	union {
		__ticketpair_t head_tail;
		struct __raw_tickets {
			__ticket_t head, tail;
		} tickets;
	};
} arch_spinlock_t;

#define __ARCH_SPIN_LOCK_UNLOCKED	{ { 0 } }

#include <asm/rwlock.h>

#endif /* ! __ASM_SPINLOCK_TYPES_H__ */
