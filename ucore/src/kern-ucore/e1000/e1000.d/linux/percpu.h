#ifndef __LINUX_PERCPU_H__
#define __LINUX_PERCPU_H__

#include <linux/preempt.h>
#include <linux/smp.h>
#include <linux/cpumask.h>
#include <linux/pfn.h>
#include <linux/init.h>

#include <asm/percpu.h>

#define per_cpu_ptr(ptr, cpu)	SHIFT_PERCPU_PTR((ptr), per_cpu_offset((cpu)))

extern void __bad_size_call_parameter(void);

#define __pcpu_size_call_return(stem, variable)				\
({	typeof(variable) pscr_ret__;					\
	__verify_pcpu_ptr(&(variable));					\
	switch(sizeof(variable)) {					\
	case 1: pscr_ret__ = stem##1(variable);break;			\
	case 2: pscr_ret__ = stem##2(variable);break;			\
	case 4: pscr_ret__ = stem##4(variable);break;			\
	case 8: pscr_ret__ = stem##8(variable);break;			\
	default:							\
		__bad_size_call_parameter();break;			\
	}								\
	pscr_ret__;							\
})

#define __pcpu_size_call(stem, variable, ...)				\
do {									\
	__verify_pcpu_ptr(&(variable));					\
	switch(sizeof(variable)) {					\
		case 1: stem##1(variable, __VA_ARGS__);break;		\
		case 2: stem##2(variable, __VA_ARGS__);break;		\
		case 4: stem##4(variable, __VA_ARGS__);break;		\
		case 8: stem##8(variable, __VA_ARGS__);break;		\
		default: 						\
			__bad_size_call_parameter();break;		\
	}								\
} while (0)

# define this_cpu_read(pcp)	__pcpu_size_call_return(this_cpu_read_, (pcp))

# define this_cpu_write(pcp, val)	__pcpu_size_call(this_cpu_write_, (pcp), (val))

# define this_cpu_add(pcp, val)		__pcpu_size_call(this_cpu_add_, (pcp), (val))

#ifndef this_cpu_sub
# define this_cpu_sub(pcp, val)		this_cpu_add((pcp), -(val))
#endif

#ifndef this_cpu_inc
# define this_cpu_inc(pcp)		this_cpu_add((pcp), 1)
#endif

#ifndef this_cpu_dec
# define this_cpu_dec(pcp)		this_cpu_sub((pcp), 1)
#endif

# define __this_cpu_read(pcp)	__pcpu_size_call_return(__this_cpu_read_, (pcp))

# define __this_cpu_add(pcp, val)	__pcpu_size_call(__this_cpu_add_, (pcp), (val))

#ifndef __this_cpu_inc
# define __this_cpu_inc(pcp)		__this_cpu_add((pcp), 1)
#endif

#define alloc_percpu(type)      \
    kmalloc(sizeof(type), 0)
//        (typeof(type) __percpu *)__alloc_percpu(sizeof(type), __alignof__(type))

#endif /* ! __LINUX_PERCPU_H__ */
