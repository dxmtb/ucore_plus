#ifndef __LINUX_PAGE_FLAGS_LAYOUT_H__
#define __LINUX_PAGE_FLAGS_LAYOUT_H__

#include <linux/numa.h>
#include <generated/bounds.h>

#define ZONES_SHIFT 2

#include <asm/sparsemem.h>

#define SECTIONS_SHIFT	(MAX_PHYSMEM_BITS - SECTION_SIZE_BITS)

#define SECTIONS_WIDTH		0

#define ZONES_WIDTH		ZONES_SHIFT

#define NODES_WIDTH		NODES_SHIFT

#define LAST_NID_SHIFT 0

#define LAST_NID_WIDTH LAST_NID_SHIFT

#endif /* ! __LINUX_PAGE_FLAGS_LAYOUT_H__ */
