#ifndef __LINUX_TIMER_H__
#define __LINUX_TIMER_H__

#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/stddef.h>
#include <linux/debugobjects.h>
#include <linux/stringify.h>

struct tvec_base;

struct timer_list {
	/*
	 * All fields that change during normal runtime grouped to the
	 * same cacheline
	 */
	struct list_head entry;
	unsigned long expires;
	struct tvec_base *base;

	void (*function)(unsigned long);
	unsigned long data;

	int slack;

#ifdef CONFIG_TIMER_STATS
	int start_pid;
	void *start_site;
	char start_comm[16];
#endif
#ifdef CONFIG_LOCKDEP
	struct lockdep_map lockdep_map;
#endif
};

#define TIMER_IRQSAFE			0x2LU

void init_timer_key(struct timer_list *timer, unsigned int flags,
		    const char *name, struct lock_class_key *key);

#define __init_timer(_timer, _flags)					\
	init_timer_key((_timer), (_flags), NULL, NULL)

#define __setup_timer(_timer, _fn, _data, _flags)			\
	do {								\
		__init_timer((_timer), (_flags));			\
		(_timer)->function = (_fn);				\
		(_timer)->data = (_data);				\
	} while (0)

extern int del_timer(struct timer_list * timer);

extern int timer_stats_active;

extern void __timer_stats_timer_set_start_info(struct timer_list *timer,
					       void *addr);

struct hrtimer;

#endif /* ! __LINUX_TIMER_H__ */
