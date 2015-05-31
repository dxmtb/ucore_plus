#ifndef __ASM_PERCPU_H__
#define __ASM_PERCPU_H__

#define __percpu_seg		gs

#include <linux/kernel.h>
#include <linux/stringify.h>

#define __percpu_prefix		"%%"__stringify(__percpu_seg)":"
#define __my_cpu_offset		this_cpu_read(this_cpu_off)

/*
 * Compared to the generic __my_cpu_offset version, the following
 * saves one instruction and avoids clobbering a temp register.
 */
#define __this_cpu_ptr(ptr)				\
({							\
	unsigned long tcp_ptr__;			\
	__verify_pcpu_ptr(ptr);				\
	asm volatile("add " __percpu_arg(1) ", %0"	\
		     : "=r" (tcp_ptr__)			\
		     : "m" (this_cpu_off), "0" (ptr));	\
	(typeof(*(ptr)) __kernel __force *)tcp_ptr__;	\
})

#define __percpu_arg(x)		__percpu_prefix "%P" #x

/*
 * Initialized pointers to per-cpu variables needed for the boot
 * processor need to use these macros to get the proper address
 * offset from __per_cpu_load on SMP.
 *
 * There also must be an entry in vmlinux_64.lds.S
 */
#define DECLARE_INIT_PER_CPU(var) \
       extern typeof(var) init_per_cpu_var(var)

#define init_per_cpu_var(var)  init_per_cpu__##var

/* For arch-specific code, we can use direct single-insn ops (they
 * don't give an lvalue though). */
extern void __bad_percpu_size(void);

#define percpu_to_op(op, var, val)			\
do {							\
	typedef typeof(var) pto_T__;			\
	if (0) {					\
		pto_T__ pto_tmp__;			\
		pto_tmp__ = (val);			\
		(void)pto_tmp__;			\
	}						\
	switch (sizeof(var)) {				\
	case 1:						\
		asm(op "b %1,"__percpu_arg(0)		\
		    : "+m" (var)			\
		    : "qi" ((pto_T__)(val)));		\
		break;					\
	case 2:						\
		asm(op "w %1,"__percpu_arg(0)		\
		    : "+m" (var)			\
		    : "ri" ((pto_T__)(val)));		\
		break;					\
	case 4:						\
		asm(op "l %1,"__percpu_arg(0)		\
		    : "+m" (var)			\
		    : "ri" ((pto_T__)(val)));		\
		break;					\
	case 8:						\
		asm(op "q %1,"__percpu_arg(0)		\
		    : "+m" (var)			\
		    : "re" ((pto_T__)(val)));		\
		break;					\
	default: __bad_percpu_size();			\
	}						\
} while (0)

/*
 * Generate a percpu add to memory instruction and optimize code
 * if one is added or subtracted.
 */
#define percpu_add_op(var, val)						\
do {									\
	typedef typeof(var) pao_T__;					\
	const int pao_ID__ = (__builtin_constant_p(val) &&		\
			      ((val) == 1 || (val) == -1)) ? (val) : 0;	\
	if (0) {							\
		pao_T__ pao_tmp__;					\
		pao_tmp__ = (val);					\
		(void)pao_tmp__;					\
	}								\
	switch (sizeof(var)) {						\
	case 1:								\
		if (pao_ID__ == 1)					\
			asm("incb "__percpu_arg(0) : "+m" (var));	\
		else if (pao_ID__ == -1)				\
			asm("decb "__percpu_arg(0) : "+m" (var));	\
		else							\
			asm("addb %1, "__percpu_arg(0)			\
			    : "+m" (var)				\
			    : "qi" ((pao_T__)(val)));			\
		break;							\
	case 2:								\
		if (pao_ID__ == 1)					\
			asm("incw "__percpu_arg(0) : "+m" (var));	\
		else if (pao_ID__ == -1)				\
			asm("decw "__percpu_arg(0) : "+m" (var));	\
		else							\
			asm("addw %1, "__percpu_arg(0)			\
			    : "+m" (var)				\
			    : "ri" ((pao_T__)(val)));			\
		break;							\
	case 4:								\
		if (pao_ID__ == 1)					\
			asm("incl "__percpu_arg(0) : "+m" (var));	\
		else if (pao_ID__ == -1)				\
			asm("decl "__percpu_arg(0) : "+m" (var));	\
		else							\
			asm("addl %1, "__percpu_arg(0)			\
			    : "+m" (var)				\
			    : "ri" ((pao_T__)(val)));			\
		break;							\
	case 8:								\
		if (pao_ID__ == 1)					\
			asm("incq "__percpu_arg(0) : "+m" (var));	\
		else if (pao_ID__ == -1)				\
			asm("decq "__percpu_arg(0) : "+m" (var));	\
		else							\
			asm("addq %1, "__percpu_arg(0)			\
			    : "+m" (var)				\
			    : "re" ((pao_T__)(val)));			\
		break;							\
	default: __bad_percpu_size();					\
	}								\
} while (0)

#define percpu_from_op(op, var, constraint)		\
({							\
	typeof(var) pfo_ret__;				\
	switch (sizeof(var)) {				\
	case 1:						\
		asm(op "b "__percpu_arg(1)",%0"		\
		    : "=q" (pfo_ret__)			\
		    : constraint);			\
		break;					\
	case 2:						\
		asm(op "w "__percpu_arg(1)",%0"		\
		    : "=r" (pfo_ret__)			\
		    : constraint);			\
		break;					\
	case 4:						\
		asm(op "l "__percpu_arg(1)",%0"		\
		    : "=r" (pfo_ret__)			\
		    : constraint);			\
		break;					\
	case 8:						\
		asm(op "q "__percpu_arg(1)",%0"		\
		    : "=r" (pfo_ret__)			\
		    : constraint);			\
		break;					\
	default: __bad_percpu_size();			\
	}						\
	pfo_ret__;					\
})

/*
 * this_cpu_read() makes gcc load the percpu variable every time it is
 * accessed while this_cpu_read_stable() allows the value to be cached.
 * this_cpu_read_stable() is more efficient and can be used if its value
 * is guaranteed to be valid across cpus.  The current users include
 * get_current() and get_thread_info() both of which are actually
 * per-thread variables implemented as per-cpu variables and thus
 * stable for the duration of the respective task.
 */
#define this_cpu_read_stable(var)	percpu_from_op("mov", var, "p" (&(var)))

#define __this_cpu_read_1(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define __this_cpu_read_2(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define __this_cpu_read_4(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))

#define __this_cpu_write_1(pcp, val)	percpu_to_op("mov", (pcp), val)
#define __this_cpu_write_2(pcp, val)	percpu_to_op("mov", (pcp), val)
#define __this_cpu_write_4(pcp, val)	percpu_to_op("mov", (pcp), val)
#define __this_cpu_add_1(pcp, val)	percpu_add_op((pcp), val)
#define __this_cpu_add_2(pcp, val)	percpu_add_op((pcp), val)
#define __this_cpu_add_4(pcp, val)	percpu_add_op((pcp), val)
#define __this_cpu_and_1(pcp, val)	percpu_to_op("and", (pcp), val)
#define __this_cpu_and_2(pcp, val)	percpu_to_op("and", (pcp), val)
#define __this_cpu_and_4(pcp, val)	percpu_to_op("and", (pcp), val)
#define __this_cpu_or_1(pcp, val)	percpu_to_op("or", (pcp), val)
#define __this_cpu_or_2(pcp, val)	percpu_to_op("or", (pcp), val)
#define __this_cpu_or_4(pcp, val)	percpu_to_op("or", (pcp), val)
#define __this_cpu_xor_1(pcp, val)	percpu_to_op("xor", (pcp), val)
#define __this_cpu_xor_2(pcp, val)	percpu_to_op("xor", (pcp), val)
#define __this_cpu_xor_4(pcp, val)	percpu_to_op("xor", (pcp), val)
#define __this_cpu_xchg_1(pcp, val)	percpu_xchg_op(pcp, val)
#define __this_cpu_xchg_2(pcp, val)	percpu_xchg_op(pcp, val)
#define __this_cpu_xchg_4(pcp, val)	percpu_xchg_op(pcp, val)

#define this_cpu_read_1(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define this_cpu_read_2(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define this_cpu_read_4(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define this_cpu_write_1(pcp, val)	percpu_to_op("mov", (pcp), val)
#define this_cpu_write_2(pcp, val)	percpu_to_op("mov", (pcp), val)
#define this_cpu_write_4(pcp, val)	percpu_to_op("mov", (pcp), val)
#define this_cpu_add_1(pcp, val)	percpu_add_op((pcp), val)
#define this_cpu_add_2(pcp, val)	percpu_add_op((pcp), val)
#define this_cpu_add_4(pcp, val)	percpu_add_op((pcp), val)
#define this_cpu_and_1(pcp, val)	percpu_to_op("and", (pcp), val)
#define this_cpu_and_2(pcp, val)	percpu_to_op("and", (pcp), val)
#define this_cpu_and_4(pcp, val)	percpu_to_op("and", (pcp), val)
#define this_cpu_or_1(pcp, val)		percpu_to_op("or", (pcp), val)
#define this_cpu_or_2(pcp, val)		percpu_to_op("or", (pcp), val)
#define this_cpu_or_4(pcp, val)		percpu_to_op("or", (pcp), val)
#define this_cpu_xor_1(pcp, val)	percpu_to_op("xor", (pcp), val)
#define this_cpu_xor_2(pcp, val)	percpu_to_op("xor", (pcp), val)
#define this_cpu_xor_4(pcp, val)	percpu_to_op("xor", (pcp), val)
#define this_cpu_xchg_1(pcp, nval)	percpu_xchg_op(pcp, nval)
#define this_cpu_xchg_2(pcp, nval)	percpu_xchg_op(pcp, nval)
#define this_cpu_xchg_4(pcp, nval)	percpu_xchg_op(pcp, nval)

#define __this_cpu_add_return_1(pcp, val) percpu_add_return_op(pcp, val)
#define __this_cpu_add_return_2(pcp, val) percpu_add_return_op(pcp, val)
#define __this_cpu_add_return_4(pcp, val) percpu_add_return_op(pcp, val)
#define __this_cpu_cmpxchg_1(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
#define __this_cpu_cmpxchg_2(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
#define __this_cpu_cmpxchg_4(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)

#define this_cpu_add_return_1(pcp, val)	percpu_add_return_op(pcp, val)
#define this_cpu_add_return_2(pcp, val)	percpu_add_return_op(pcp, val)
#define this_cpu_add_return_4(pcp, val)	percpu_add_return_op(pcp, val)
#define this_cpu_cmpxchg_1(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
#define this_cpu_cmpxchg_2(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
#define this_cpu_cmpxchg_4(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)

#define __this_cpu_cmpxchg_double_4	percpu_cmpxchg8b_double
#define this_cpu_cmpxchg_double_4	percpu_cmpxchg8b_double

#define __this_cpu_read_8(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define __this_cpu_write_8(pcp, val)	percpu_to_op("mov", (pcp), val)
#define __this_cpu_add_8(pcp, val)	percpu_add_op((pcp), val)
#define __this_cpu_and_8(pcp, val)	percpu_to_op("and", (pcp), val)
#define __this_cpu_or_8(pcp, val)	percpu_to_op("or", (pcp), val)
#define __this_cpu_xor_8(pcp, val)	percpu_to_op("xor", (pcp), val)
#define __this_cpu_add_return_8(pcp, val) percpu_add_return_op(pcp, val)
#define __this_cpu_xchg_8(pcp, nval)	percpu_xchg_op(pcp, nval)
#define __this_cpu_cmpxchg_8(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)

#define this_cpu_read_8(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define this_cpu_write_8(pcp, val)	percpu_to_op("mov", (pcp), val)
#define this_cpu_add_8(pcp, val)	percpu_add_op((pcp), val)
#define this_cpu_and_8(pcp, val)	percpu_to_op("and", (pcp), val)
#define this_cpu_or_8(pcp, val)		percpu_to_op("or", (pcp), val)
#define this_cpu_xor_8(pcp, val)	percpu_to_op("xor", (pcp), val)
#define this_cpu_add_return_8(pcp, val)	percpu_add_return_op(pcp, val)
#define this_cpu_xchg_8(pcp, nval)	percpu_xchg_op(pcp, nval)
#define this_cpu_cmpxchg_8(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)

#define __this_cpu_cmpxchg_double_8	percpu_cmpxchg16b_double
#define this_cpu_cmpxchg_double_8	percpu_cmpxchg16b_double

#include <asm-generic/percpu.h>

#define DECLARE_EARLY_PER_CPU(_type, _name)			\
	DECLARE_PER_CPU(_type, _name);				\
	extern __typeof__(_type) *_name##_early_ptr;		\
	extern __typeof__(_type)  _name##_early_map[]

#define DECLARE_EARLY_PER_CPU_READ_MOSTLY(_type, _name)		\
	DECLARE_PER_CPU_READ_MOSTLY(_type, _name);		\
	extern __typeof__(_type) *_name##_early_ptr;		\
	extern __typeof__(_type)  _name##_early_map[]

#define	early_per_cpu_ptr(_name) (_name##_early_ptr)

#define	early_per_cpu(_name, _cpu) 				\
	*(early_per_cpu_ptr(_name) ?				\
		&early_per_cpu_ptr(_name)[_cpu] :		\
		&per_cpu(_name, _cpu))

#endif /* ! __ASM_PERCPU_H__ */
