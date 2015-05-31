#ifndef __LINUX_EXPORT_H__
#define __LINUX_EXPORT_H__

struct kernel_symbol
{
	unsigned long value;
	const char *name;
};

#define THIS_MODULE ((struct module *)0)

#endif /* ! __LINUX_EXPORT_H__ */
