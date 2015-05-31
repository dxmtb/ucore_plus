#ifndef __LINUX_LOCKDEP_H__
#define __LINUX_LOCKDEP_H__

struct task_struct;

# define lockdep_init_map(lock, name, key, sub) \
		do { (void)(name); (void)(key); } while (0)
# define lockdep_set_class(lock, key)		do { (void)(key); } while (0)

/*
 * The class key takes no space if lockdep is disabled:
 */
struct lock_class_key { };

#define LOCK_CONTENDED(_lock, try, lock) \
	lock(_lock)

#define LOCK_CONTENDED_FLAGS(_lock, try, lock, lockfl, flags) \
	lockfl((_lock), (flags))

/*
 * For trivial one-depth nesting of a lock-class, the following
 * global define can be used. (Subsystems with multiple levels
 * of nesting should define their own lock-nesting subclasses.)
 */
#define SINGLE_DEPTH_NESTING			1

# define spin_acquire(l, s, t, i)		do { } while (0)
# define spin_release(l, n, i)			do { } while (0)

# define rwlock_acquire(l, s, t, i)		do { } while (0)
# define rwlock_acquire_read(l, s, t, i)	do { } while (0)
# define rwlock_release(l, n, i)		do { } while (0)

#endif /* ! __LINUX_LOCKDEP_H__ */
