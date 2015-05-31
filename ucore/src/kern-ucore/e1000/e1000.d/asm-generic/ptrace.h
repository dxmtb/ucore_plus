#ifndef __ASM_GENERIC_PTRACE_H__
#define __ASM_GENERIC_PTRACE_H__

#ifndef SET_IP
#define SET_IP(regs, val) (GET_IP(regs) = (val))
#endif

#ifndef SET_USP
#define SET_USP(regs, val) (GET_USP(regs) = (val))
#endif

#ifndef SET_FP
#define SET_FP(regs, val) (GET_FP(regs) = (val))
#endif

#endif /* ! __ASM_GENERIC_PTRACE_H__ */
