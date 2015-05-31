#ifndef __LINUX_TRACEPOINT_H__
#define __LINUX_TRACEPOINT_H__

#include <linux/errno.h>
#include <linux/types.h>
#include <linux/rcupdate.h>
#include <linux/static_key.h>

struct module;
struct tracepoint;

struct tracepoint_func {
	void *func;
	void *data;
};

struct tracepoint {
	const char *name;		/* Tracepoint name */
	struct static_key key;
	void (*regfunc)(void);
	void (*unregfunc)(void);
	struct tracepoint_func __rcu *funcs;
};

#endif /* ! __LINUX_TRACEPOINT_H__ */
