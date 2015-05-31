#ifndef __LINUX_SMP_H__
#define __LINUX_SMP_H__

#include <linux/errno.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/cpumask.h>
#include <linux/init.h>
#include <linux/irqflags.h>

#include <linux/preempt.h>
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/thread_info.h>
#include <asm/smp.h>

# define smp_processor_id() raw_smp_processor_id()

#endif /* ! __LINUX_SMP_H__ */
