#ifndef __LINUX_PCI_ASPM_H__
#define __LINUX_PCI_ASPM_H__

#include <linux/pci.h>

void pcie_aspm_init_link_state(struct pci_dev *pdev);

#endif /* ! __LINUX_PCI_ASPM_H__ */
