#ifndef __LINUX_COMPILER_GCC4_H__
#define __LINUX_COMPILER_GCC4_H__

#define __used			__attribute__((__used__))
#define __must_check 		__attribute__((warn_unused_result))
#define __compiler_offsetof(a,b) __builtin_offsetof(a,b)

# define __compiletime_object_size(obj) __builtin_object_size(obj, 0)

#endif /* ! __LINUX_COMPILER_GCC4_H__ */
