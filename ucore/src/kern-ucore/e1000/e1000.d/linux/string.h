#ifndef __LINUX_STRING_H__
#define __LINUX_STRING_H__

#include <linux/compiler.h>	/* for inline */
#include <linux/types.h>	/* for size_t */
#include <linux/stddef.h>	/* for NULL */

#include <uapi/linux/string.h>

/*
 * Include machine specific inline routines
 */
#include <asm/string.h>

extern char * strcpy(char *,const char *);

extern char * strncpy(char *,const char *, __kernel_size_t);

size_t strlcpy(char *, const char *, size_t);

extern int strncmp(const char *,const char *,__kernel_size_t);

extern char * strchr(const char *,int);

extern char * strrchr(const char *,int);

extern char *strim(char *);

extern char * strstr(const char *, const char *);

extern __kernel_size_t strlen(const char *);

extern int memcmp(const void *,const void *,__kernel_size_t);

#endif /* ! __LINUX_STRING_H__ */
