#ifndef __LINUX_STDDEF_H__
#define __LINUX_STDDEF_H__

#include <uapi/linux/stddef.h>

#define NULL ((void *)0)

enum {
	false	= 0,
	true	= 1
};

#define offsetof(TYPE,MEMBER) __compiler_offsetof(TYPE,MEMBER)

#endif /* ! __LINUX_STDDEF_H__ */
