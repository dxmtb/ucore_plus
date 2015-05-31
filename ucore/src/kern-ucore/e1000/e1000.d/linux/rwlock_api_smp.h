#ifndef __LINUX_RWLOCK_API_SMP_H__
#define __LINUX_RWLOCK_API_SMP_H__

void __lockfunc _raw_write_lock(rwlock_t *lock)		__acquires(lock);

void __lockfunc _raw_write_lock_bh(rwlock_t *lock)	__acquires(lock);

void __lockfunc _raw_write_unlock_bh(rwlock_t *lock)	__releases(lock);

#define _raw_write_unlock(lock) __raw_write_unlock(lock)

static inline void __raw_write_unlock(rwlock_t *lock)
{
	rwlock_release(&lock->dep_map, 1, _RET_IP_);
	do_raw_write_unlock(lock);
	preempt_enable();
}

#endif /* ! __LINUX_RWLOCK_API_SMP_H__ */
