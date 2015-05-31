#ifndef __LINUX_PROC_FS_H__
#define __LINUX_PROC_FS_H__

#include <linux/types.h>
#include <linux/fs.h>

struct proc_dir_entry;

extern struct proc_dir_entry *proc_mkdir_data(const char *, umode_t,
					      struct proc_dir_entry *, void *);

extern struct proc_dir_entry *proc_create_data(const char *, umode_t,
					       struct proc_dir_entry *,
					       const struct file_operations *,
					       void *);

#endif /* ! __LINUX_PROC_FS_H__ */
