#ifndef __LINUX_SYSCTL_H__
#define __LINUX_SYSCTL_H__

#include <linux/list.h>
#include <linux/rcupdate.h>
#include <linux/wait.h>
#include <linux/rbtree.h>
#include <uapi/linux/sysctl.h>

struct ctl_table;
struct nsproxy;
struct ctl_table_root;
struct ctl_table_header;
struct ctl_dir;

typedef int proc_handler (struct ctl_table *ctl, int write,
			  void __user *buffer, size_t *lenp, loff_t *ppos);

struct ctl_table_poll {
	atomic_t event;
	wait_queue_head_t wait;
};

struct ctl_table
{
	const char *procname;		/* Text ID for /proc/sys, or zero */
	void *data;
	int maxlen;
	umode_t mode;
	struct ctl_table *child;	/* Deprecated */
	proc_handler *proc_handler;	/* Callback for text formatting */
	struct ctl_table_poll *poll;
	void *extra1;
	void *extra2;
};

struct ctl_node {
	struct rb_node node;
	struct ctl_table_header *header;
};

/* struct ctl_table_header is used to maintain dynamic lists of
   struct ctl_table trees. */
struct ctl_table_header
{
	union {
		struct {
			struct ctl_table *ctl_table;
			int used;
			int count;
			int nreg;
		};
		struct rcu_head rcu;
	};
	struct completion *unregistering;
	struct ctl_table *ctl_table_arg;
	struct ctl_table_root *root;
	struct ctl_table_set *set;
	struct ctl_dir *parent;
	struct ctl_node *node;
};

struct ctl_dir {
	/* Header must be at the start of ctl_dir */
	struct ctl_table_header header;
	struct rb_root root;
};

struct ctl_table_set {
	int (*is_seen)(struct ctl_table_set *);
	struct ctl_dir dir;
};

struct ctl_table_root {
	struct ctl_table_set default_set;
	struct ctl_table_set *(*lookup)(struct ctl_table_root *root,
					   struct nsproxy *namespaces);
	int (*permissions)(struct ctl_table_header *head, struct ctl_table *table);
};

#endif /* ! __LINUX_SYSCTL_H__ */
