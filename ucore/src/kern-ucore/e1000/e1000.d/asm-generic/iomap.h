#ifndef __ASM_GENERIC_IOMAP_H__
#define __ASM_GENERIC_IOMAP_H__

#include <linux/linkage.h>
#include <asm/byteorder.h>

extern void ioread16_rep(void __iomem *port, void *buf, unsigned long count);

extern void iowrite16_rep(void __iomem *port, const void *buf, unsigned long count);

struct pci_dev;

#include <asm-generic/pci_iomap.h>

#endif /* ! __ASM_GENERIC_IOMAP_H__ */
