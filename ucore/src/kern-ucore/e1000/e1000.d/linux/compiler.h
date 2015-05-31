#ifndef __LINUX_COMPILER_H__
#define __LINUX_COMPILER_H__

#define __LINUX_COMPILER_H

# define __user
# define __kernel

# define __force
# define __nocast
# define __iomem
# define __chk_user_ptr(x) (void)0

# define __acquires(x)
# define __releases(x)
# define __acquire(x) (void)0
# define __release(x) (void)0
# define __cond_lock(x,c) (c)
# define __percpu
# define __rcu

#define ___PASTE(a,b) a##b
#define __PASTE(a,b) ___PASTE(a,b)

#include <linux/compiler-gcc.h>

#define notrace __attribute__((no_instrument_function))

# define likely(x)	__builtin_expect(!!(x), 1)
# define unlikely(x)	__builtin_expect(!!(x), 0)

#ifndef unreachable
# define unreachable() do { } while (1)
#endif

#ifndef __UNIQUE_ID
# define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __LINE__)
#endif

#undef __deprecated
#undef __deprecated_for_modules
#define __deprecated

#ifndef __cold
#define __cold
#endif

#ifndef __section
# define __section(S) __attribute__ ((__section__(#S)))
#endif

#ifndef __same_type
# define __same_type(a, b) __builtin_types_compatible_p(typeof(a), typeof(b))
#endif

# define __compiletime_error(message)

/*
 * Prevent the compiler from merging or refetching accesses.  The compiler
 * is also forbidden from reordering successive instances of ACCESS_ONCE(),
 * but only when the compiler is aware of some particular ordering.  One way
 * to make the compiler aware of ordering is to put the two invocations of
 * ACCESS_ONCE() in different C statements.
 *
 * This macro does absolutely -nothing- to prevent the CPU from reordering,
 * merging, or refetching absolutely anything at any time.  Its main intended
 * use is to mediate communication between process-level code and irq/NMI
 * handlers, all running on the same CPU.
 */
#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))

#endif /* ! __LINUX_COMPILER_H__ */
