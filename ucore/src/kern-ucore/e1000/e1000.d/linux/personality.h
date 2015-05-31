#ifndef __LINUX_PERSONALITY_H__
#define __LINUX_PERSONALITY_H__

#include <uapi/linux/personality.h>

struct exec_domain;
struct pt_regs;

/*
 * Description of an execution domain.
 *
 * The first two members are refernced from assembly source
 * and should stay where they are unless explicitly needed.
 */
typedef void (*handler_t)(int, struct pt_regs *);

struct exec_domain {
	const char		*name;		/* name of the execdomain */
	handler_t		handler;	/* handler for syscalls */
	unsigned char		pers_low;	/* lowest personality */
	unsigned char		pers_high;	/* highest personality */
	unsigned long		*signal_map;	/* signal mapping */
	unsigned long		*signal_invmap;	/* reverse signal mapping */
	struct map_segment	*err_map;	/* error mapping */
	struct map_segment	*socktype_map;	/* socket type mapping */
	struct map_segment	*sockopt_map;	/* socket option mapping */
	struct map_segment	*af_map;	/* address family mapping */
	struct module		*module;	/* module context of the ed. */
	struct exec_domain	*next;		/* linked list (internal) */
};

#endif /* ! __LINUX_PERSONALITY_H__ */
