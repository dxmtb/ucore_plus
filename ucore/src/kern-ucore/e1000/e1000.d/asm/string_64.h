#ifndef __ASM_STRING_64_H__
#define __ASM_STRING_64_H__

#define __HAVE_ARCH_MEMCPY 1

extern void *__memcpy(void *to, const void *from, size_t len);
#define memcpy(dst, src, len)					\
({								\
	size_t __len = (len);					\
	void *__ret;						\
	if (__builtin_constant_p(len) && __len >= 64)		\
		__ret = __memcpy((dst), (src), __len);		\
	else							\
		__ret = __builtin_memcpy((dst), (src), __len);	\
	__ret;							\
})

#define __HAVE_ARCH_MEMSET
void *memset(void *s, int c, size_t n);

#define __HAVE_ARCH_MEMMOVE
void *memmove(void *dest, const void *src, size_t count);

int memcmp(const void *cs, const void *ct, size_t count);
size_t strlen(const char *s);
char *strcpy(char *dest, const char *src);

#endif /* ! __ASM_STRING_64_H__ */
