#ifndef __ASM_SIGNAL_H__
#define __ASM_SIGNAL_H__

#include <linux/linkage.h>

#define _NSIG		64

# define _NSIG_BPW	64

#define _NSIG_WORDS	(_NSIG / _NSIG_BPW)

typedef struct {
	unsigned long sig[_NSIG_WORDS];
} sigset_t;

#include <uapi/asm/signal.h>

#define __ARCH_HAS_SA_RESTORER

#include <asm/sigcontext.h>

#endif /* ! __ASM_SIGNAL_H__ */
