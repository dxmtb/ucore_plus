#ifndef __LINUX_UPROBES_H__
#define __LINUX_UPROBES_H__

#include <linux/errno.h>
#include <linux/rbtree.h>

struct vm_area_struct;
struct mm_struct;
struct inode;

# include <asm/uprobes.h>

struct uprobes_state {
};

#endif /* ! __LINUX_UPROBES_H__ */
