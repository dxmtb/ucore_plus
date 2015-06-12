#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include "e1000_dde.h"

const char e1000_dev_name[] = "e1000_pci_dev";
struct pci_dev e1000_dev;
struct net_device *netdev;

struct pci_device_id ent;

void e1000_dev_init(struct pci_func *pcif) {
    e1000_dev.vendor = PCI_VENDOR(pcif->dev_id);
    e1000_dev.device = PCI_PRODUCT(pcif->dev_id);
    e1000_dev.devfn = (pcif->dev << 3) + pcif->func;
    e1000_dev.dev.init_name = e1000_dev_name;
    pci_setup_device(&e1000_dev);
}

static inline unsigned char *__skb_push(struct sk_buff *skb, unsigned int len)
{
        skb->data -= len;
        skb->len  += len;
        return skb->data;
}

struct sk_buff *alloc_skb_from_buf(void *buf, int buf_size) {
    unsigned int size;
    struct kmem_cache *cache;
    struct skb_shared_info *shinfo;
    struct sk_buff *skb;
    u8 *data;

    /* Get the HEAD */
    skb = ucore_kmalloc(sizeof(struct sk_buff));
    if (!skb)
        goto out;

    /* We do our best to align skb_shared_info on a separate cache
     * line. It usually works because kmalloc(X > SMP_CACHE_BYTES) gives
     * aligned memory blocks, unless SLUB/SLAB debug is enabled.
     * Both skb->head and skb_shared_info are cache line aligned.
     */
    size = buf_size + sizeof(struct skb_shared_info);
    data = ucore_kmalloc(size);
    if (!data)
        goto nodata;

    /*
     * Only clear those fields we need to clear, not those that we will
     * actually initialise below. Hence, don't put any more fields after
     * the tail pointer in struct sk_buff!
     */
    memset(skb, 0, offsetof(struct sk_buff, tail));
    /* Account for allocated memory : skb + skb->head */
    skb->truesize = size;
    skb->pfmemalloc = 0; // FIXME: what value?
    skb->head = data;
    skb->data = data;
    skb_reset_tail_pointer(skb);
    skb->end = skb->tail + size;
    skb->mac_header = ~0U;
    skb->transport_header = ~0U;

    /* make sure we initialize shinfo sequentially */
    shinfo = skb_shinfo(skb);
    memset(shinfo, 0, offsetof(struct skb_shared_info, dataref));

    // put buf into skb
    __skb_push(skb, buf_size);
    memcpy(skb->data, buf, buf_size);

out:
    return skb;
nodata:
    kfree(skb);
    skb = NULL;
    goto out;
}

void e1000_xmit_buf(void *buf, unsigned int buf_size, struct net_device *netdev);

void test_transmission() {
    kprintf("Testing transmission...\n");
	uint32_t data[50];
	int i;

	for (i = 0; i < 50; i++) {
		data[i] = i;
	}

	for (i = 0; i < 18; i++) {
        //struct sk_buff *skb;
        //skb = alloc_skb_from_buf(data, 50);
        //netdev->netdev_ops->ndo_start_xmit(skb, netdev);
        e1000_xmit_buf(data, 50, netdev);
        while(1);
	}
}

int e1000_dde_init(struct pci_func *pcif) {
    e1000_dev_init(pcif);
    e1000_probe(&e1000_dev, &ent);
    netdev = e1000_dev.dev.p;
    kprintf("netdev %x\n", netdev);
    netdev->netdev_ops->ndo_open(netdev);
    test_transmission();
    return 0;
}
