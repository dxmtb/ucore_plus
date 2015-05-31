#ifndef __LINUX_KMOD_H__
#define __LINUX_KMOD_H__

#include <linux/gfp.h>
#include <linux/stddef.h>
#include <linux/errno.h>
#include <linux/compiler.h>
#include <linux/workqueue.h>
#include <linux/sysctl.h>

struct cred;
struct file;

enum umh_disable_depth {
	UMH_ENABLED = 0,
	UMH_FREEZING,
	UMH_DISABLED,
};

extern int __usermodehelper_disable(enum umh_disable_depth depth);
extern void __usermodehelper_set_disable_depth(enum umh_disable_depth depth);

#endif /* ! __LINUX_KMOD_H__ */
