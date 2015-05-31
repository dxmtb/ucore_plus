#ifndef __ASM_GENERIC_PARAM_H__
#define __ASM_GENERIC_PARAM_H__

#include <uapi/asm-generic/param.h>

# undef HZ
# define HZ		CONFIG_HZ	/* Internal kernel timer frequency */

#endif /* ! __ASM_GENERIC_PARAM_H__ */
