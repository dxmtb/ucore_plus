#ifndef __LINUX_INIT_H__
#define __LINUX_INIT_H__

#include <linux/compiler.h>
#include <linux/types.h>

/* These are for everybody (although not all archs will actually
   discard it in modules) */
#define __init		__section(.init.text) __cold notrace
#define __initdata	__section(.init.data)
#define __initconst	__constsection(.init.rodata)

#define __exit_call	__used __section(.exitcall.exit)

#define __constsection(x) __section(x)

#define __exitused  __used

#define __exit          __section(.exit.text) __exitused __cold notrace

#define __cpuinit        __section(.cpuinit.text) __cold notrace
#define __cpuinitdata    __section(.cpuinit.data)

#define __meminit        __section(.meminit.text) __cold notrace

/*
 * Used for initialization calls..
 */
typedef int (*initcall_t)(void);
typedef void (*exitcall_t)(void);

#define __define_initcall(fn, id) \
	static initcall_t __initcall_##fn##id __used \
	__attribute__((__section__(".initcall" #id ".init"))) = fn

#define device_initcall(fn)		__define_initcall(fn, 6)

#define __initcall(fn) device_initcall(fn)

#define __exitcall(fn) \
	static exitcall_t __exitcall_##fn __exit_call = fn

/**
 * module_init() - driver initialization entry point
 * @x: function to be run at kernel boot time or module insertion
 *
 * module_init() will either be called during do_initcalls() (if
 * builtin) or at module insertion time (if a module).  There can only
 * be one per module.
 */
#define module_init(x)	__initcall(x);

/**
 * module_exit() - driver exit entry point
 * @x: function to be run when driver is removed
 *
 * module_exit() will wrap the driver clean-up code
 * with cleanup_module() when used with rmmod when
 * the driver is a module.  If the driver is statically
 * compiled into the kernel, module_exit() has no effect.
 * There can only be one per module.
 */
#define module_exit(x)	__exitcall(x);

#endif /* ! __LINUX_INIT_H__ */
