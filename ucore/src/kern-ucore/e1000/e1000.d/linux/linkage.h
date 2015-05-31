#ifndef __LINUX_LINKAGE_H__
#define __LINUX_LINKAGE_H__

#include <linux/compiler.h>
#include <linux/stringify.h>
#include <linux/export.h>
#include <asm/linkage.h>

#define CPP_ASMLINKAGE

#ifndef asmlinkage
#define asmlinkage CPP_ASMLINKAGE
#endif

#endif /* ! __LINUX_LINKAGE_H__ */
