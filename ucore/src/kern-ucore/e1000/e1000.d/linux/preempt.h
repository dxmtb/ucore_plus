#ifndef __LINUX_PREEMPT_H__
#define __LINUX_PREEMPT_H__

#include <linux/thread_info.h>
#include <linux/linkage.h>
#include <linux/list.h>

# define add_preempt_count(val)	do { preempt_count() += (val); } while (0)
# define sub_preempt_count(val)	do { preempt_count() -= (val); } while (0)

#define inc_preempt_count() add_preempt_count(1)
#define dec_preempt_count() sub_preempt_count(1)

#define preempt_count()	(current_thread_info()->preempt_count)

#define preempt_check_resched()		do { } while (0)

/*
 * Even if we don't have any preemption, we need preempt disable/enable
 * to be barriers, so that we don't have things like get_user/put_user
 * that can cause faults and scheduling migrate into our preempt-protected
 * region.
 */
#define preempt_disable()		barrier()

#define preempt_enable_no_resched()	barrier()
#define preempt_enable()		barrier()

#define preempt_disable_notrace()		barrier()

#define preempt_enable_notrace()		barrier()

#endif /* ! __LINUX_PREEMPT_H__ */
