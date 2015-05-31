#ifndef __ASM_MMZONE_64_H__
#define __ASM_MMZONE_64_H__

#include <linux/mmdebug.h>
#include <asm/smp.h>

extern struct pglist_data *node_data[];

#define NODE_DATA(nid)		(node_data[nid])

#endif /* ! __ASM_MMZONE_64_H__ */
