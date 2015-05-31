#ifndef __LINUX_RCUTREE_H__
#define __LINUX_RCUTREE_H__

extern void rcu_note_context_switch(int cpu);

extern void synchronize_sched_expedited(void);

#endif /* ! __LINUX_RCUTREE_H__ */
