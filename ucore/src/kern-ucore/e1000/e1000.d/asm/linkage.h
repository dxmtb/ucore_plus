#ifndef __ASM_LINKAGE_H__
#define __ASM_LINKAGE_H__

#include <linux/stringify.h>

#undef notrace
#define notrace __attribute__((no_instrument_function))

#endif /* ! __ASM_LINKAGE_H__ */
