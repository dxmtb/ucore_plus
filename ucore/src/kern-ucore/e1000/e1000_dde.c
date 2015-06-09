#include <linux/pci.h>
#include <linux/netdevice.h>
#include "e1000_dde.h"

const char e1000_dev_name[] = "e1000_pci_dev";
struct pci_dev e1000_dev;

struct pci_device_id ent;

void e1000_dev_init(struct pci_func *pcif) {
    e1000_dev.vendor = PCI_VENDOR(pcif->dev_id);
    e1000_dev.device = PCI_PRODUCT(pcif->dev_id);
    e1000_dev.devfn = (pcif->dev << 3) + pcif->func;
    e1000_dev.dev.init_name = e1000_dev_name;
    pci_setup_device(&e1000_dev);
}

int e1000_dde_init(struct pci_func *pcif) {
	struct net_device *netdev;
    e1000_dev_init(pcif);
    e1000_probe(&e1000_dev, &ent);
    netdev = e1000_dev.dev.p;
    kprintf("netdev %x\n", netdev);
	netdev->netdev_ops->ndo_open(netdev);
    return 0;
}
