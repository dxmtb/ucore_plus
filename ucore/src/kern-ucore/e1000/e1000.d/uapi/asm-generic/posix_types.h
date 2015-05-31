#ifndef __UAPI_ASM_GENERIC_POSIX_TYPES_H__
#define __UAPI_ASM_GENERIC_POSIX_TYPES_H__

#include <asm/bitsperlong.h>

typedef long		__kernel_long_t;
typedef unsigned long	__kernel_ulong_t;

typedef int		__kernel_pid_t;

typedef __kernel_long_t		__kernel_suseconds_t;

typedef unsigned int	__kernel_uid32_t;
typedef unsigned int	__kernel_gid32_t;

typedef __kernel_ulong_t __kernel_size_t;
typedef __kernel_long_t	__kernel_ssize_t;

typedef long long	__kernel_loff_t;
typedef __kernel_long_t	__kernel_time_t;
typedef __kernel_long_t	__kernel_clock_t;
typedef int		__kernel_timer_t;
typedef int		__kernel_clockid_t;

#endif /* ! __UAPI_ASM_GENERIC_POSIX_TYPES_H__ */
