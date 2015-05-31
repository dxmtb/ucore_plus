#ifndef __LINUX_SEQ_FILE_H__
#define __LINUX_SEQ_FILE_H__

#include <linux/types.h>
#include <linux/string.h>
#include <linux/bug.h>
#include <linux/mutex.h>
#include <linux/cpumask.h>
#include <linux/nodemask.h>

struct seq_operations;
struct file;
struct path;
struct inode;
struct dentry;
struct user_namespace;

struct seq_file {
	char *buf;
	size_t size;
	size_t from;
	size_t count;
	loff_t index;
	loff_t read_pos;
	u64 version;
	struct mutex lock;
	const struct seq_operations *op;
	int poll_event;
#ifdef CONFIG_USER_NS
	struct user_namespace *user_ns;
#endif
	void *private;
};

struct seq_operations {
	void * (*start) (struct seq_file *m, loff_t *pos);
	void (*stop) (struct seq_file *m, void *v);
	void * (*next) (struct seq_file *m, void *v, loff_t *pos);
	int (*show) (struct seq_file *m, void *v);
};

int seq_bitmap(struct seq_file *m, const unsigned long *bits,
				   unsigned int nr_bits);

int seq_bitmap_list(struct seq_file *m, const unsigned long *bits,
		unsigned int nr_bits);

#endif /* ! __LINUX_SEQ_FILE_H__ */
