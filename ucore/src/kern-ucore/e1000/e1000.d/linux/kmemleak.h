#ifndef __LINUX_KMEMLEAK_H__
#define __LINUX_KMEMLEAK_H__

static inline void kmemleak_alloc(const void *ptr, size_t size, int min_count,
				  gfp_t gfp)
{
}

#endif /* ! __LINUX_KMEMLEAK_H__ */
