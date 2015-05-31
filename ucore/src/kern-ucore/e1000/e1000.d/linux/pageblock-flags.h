#ifndef __LINUX_PAGEBLOCK_FLAGS_H__
#define __LINUX_PAGEBLOCK_FLAGS_H__

#include <linux/types.h>

struct page;

unsigned long get_pageblock_flags_group(struct page *page,
					int start_bitidx, int end_bitidx);

#endif /* ! __LINUX_PAGEBLOCK_FLAGS_H__ */
