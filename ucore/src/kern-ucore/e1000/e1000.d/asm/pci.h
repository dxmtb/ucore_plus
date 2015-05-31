#ifndef __ASM_PCI_H__
#define __ASM_PCI_H__

#include <linux/mm.h> /* for struct page */
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <asm/scatterlist.h>
#include <asm/io.h>
#include <asm/x86_init.h>

struct pci_sysdata {
	int		domain;		/* PCI domain */
	int		node;		/* NUMA node */
#ifdef CONFIG_ACPI
	void		*acpi;		/* ACPI-specific data */
#endif
#ifdef CONFIG_X86_64
	void		*iommu;		/* IOMMU private data */
#endif
};

static inline int pci_domain_nr(struct pci_bus *bus)
{
	struct pci_sysdata *sd = bus->sysdata;
	return sd->domain;
}

struct msi_desc;

#define PCI_DMA_BUS_IS_PHYS (dma_ops->is_phys)

#include <asm/pci_64.h>

#include <asm-generic/pci-dma-compat.h>

#include <asm-generic/pci.h>
#define PCIBIOS_MAX_MEM_32 0xffffffff

static inline int __pcibus_to_node(const struct pci_bus *bus)
{
	const struct pci_sysdata *sd = bus->sysdata;

	return sd->node;
}

#endif /* ! __ASM_PCI_H__ */
