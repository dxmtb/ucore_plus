#ifndef __LINUX_PID_H__
#define __LINUX_PID_H__

#include <linux/rcupdate.h>

enum pid_type
{
	PIDTYPE_PID,
	PIDTYPE_PGID,
	PIDTYPE_SID,
	PIDTYPE_MAX
};

struct upid {
	/* Try to keep pid_chain in the same cacheline as nr for find_vpid */
	int nr;
	struct pid_namespace *ns;
	struct hlist_node pid_chain;
};

struct pid
{
	atomic_t count;
	unsigned int level;
	/* lists of tasks that use this pid */
	struct hlist_head tasks[PIDTYPE_MAX];
	struct rcu_head rcu;
	struct upid numbers[1];
};

struct pid_link
{
	struct hlist_node node;
	struct pid *pid;
};

static inline struct pid *get_pid(struct pid *pid)
{
	if (pid)
		atomic_inc(&pid->count);
	return pid;
}

extern void put_pid(struct pid *pid);

struct pid_namespace;
extern struct pid_namespace init_pid_ns;

pid_t pid_vnr(struct pid *pid);

#endif /* ! __LINUX_PID_H__ */
