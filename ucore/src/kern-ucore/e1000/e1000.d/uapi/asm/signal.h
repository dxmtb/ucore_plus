#ifndef __UAPI_ASM_SIGNAL_H__
#define __UAPI_ASM_SIGNAL_H__

#include <linux/types.h>
#include <linux/time.h>
#include <linux/compiler.h>

struct siginfo;

#define SIGKILL		 9

#define SIGRTMIN	32

#define SA_ONSTACK	0x08000000u

#include <asm-generic/signal-defs.h>

#endif /* ! __UAPI_ASM_SIGNAL_H__ */
