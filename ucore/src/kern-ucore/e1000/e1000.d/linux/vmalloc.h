#ifndef __LINUX_VMALLOC_H__
#define __LINUX_VMALLOC_H__

#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/list.h>
#include <asm/page.h>		/* pgprot_t */
#include <linux/rbtree.h>

struct vm_area_struct;		/* vma defining user mapping in mm_types.h */

extern void *vzalloc(unsigned long size);

extern void vfree(const void *addr);

#endif /* ! __LINUX_VMALLOC_H__ */
