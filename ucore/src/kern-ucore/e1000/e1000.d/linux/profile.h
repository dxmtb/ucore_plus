#ifndef __LINUX_PROFILE_H__
#define __LINUX_PROFILE_H__

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cpumask.h>
#include <linux/cache.h>

#include <asm/errno.h>

struct proc_dir_entry;
struct pt_regs;
struct notifier_block;

extern int prof_on __read_mostly;

/*
 * Add multiple profiler hits to a given address:
 */
void profile_hits(int type, void *ip, unsigned int nr_hits);

struct task_struct;
struct mm_struct;

struct pt_regs;

#endif /* ! __LINUX_PROFILE_H__ */
