#ifndef __LINUX_SEM_H__
#define __LINUX_SEM_H__

#include <linux/atomic.h>
#include <linux/rcupdate.h>
#include <linux/cache.h>
#include <uapi/linux/sem.h>

struct task_struct;

struct sysv_sem {
	struct sem_undo_list *undo_list;
};

#endif /* ! __LINUX_SEM_H__ */
