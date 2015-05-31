#include <asm/checksum_64.h>
//#include <asm/dma-mapping.h>
#include <asm/io.h>
#include <asm/page_64.h>
#include <asm/smp.h>
#include <asm/string_64.h>
#include <asm/thread_info.h>
#include <asm-generic/bitops/find.h>
#include <asm-generic/bug.h>
#include <asm-generic/delay.h>
#include <asm-generic/iomap.h>
#include <linux/bottom_half.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/dynamic_queue_limits.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/gfp.h>
#include <linux/hardirq.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/mutex.h>
#include <linux/net.h>
#include <linux/netdevice.h>
#include <linux/pci.h>
#include <linux/pm_wakeup.h>
#include <linux/printk.h>
#include <linux/rtnetlink.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/slub_def.h>
#include <linux/spinlock_api_smp.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/vmalloc.h>
#include <linux/workqueue.h>
#include <stdarg.h>

#define DDE_WEAK __attribute__((weak))

#define dde_dummy_printf(...) kprintf(__VA_ARGS__)
#define dde_printf(...) dde_dummy_printf(__VA_ARGS__)

/*
 */
DDE_WEAK struct net_device * alloc_etherdev_mqs(int a, unsigned int b, unsigned int c) {
	dde_printf("alloc_etherdev_mqs not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK struct page * alloc_pages_current(gfp_t a, unsigned int b) {
	dde_printf("alloc_pages_current not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK struct sk_buff * __alloc_skb(unsigned int a, gfp_t b, int c, int d) {
	dde_printf("__alloc_skb not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK bool cancel_delayed_work_sync(struct delayed_work * a) {
	dde_printf("cancel_delayed_work_sync not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK bool cancel_work_sync(struct work_struct * a) {
	dde_printf("cancel_work_sync not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void __const_udelay(unsigned long a) {
	dde_printf("__const_udelay not implemented\n");
}

/*
 */
DDE_WEAK void consume_skb(struct sk_buff * a) {
	dde_printf("consume_skb not implemented\n");
}

typeof(int) cpu_number;

/*
 */
DDE_WEAK __sum16 csum_ipv6_magic(const struct in6_addr * a, const struct in6_addr * b, __u32 c, unsigned short d, __wsum e) {
	dde_printf("csum_ipv6_magic not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void delayed_work_timer_fn(unsigned long a) {
	dde_printf("delayed_work_timer_fn not implemented\n");
}

/*
 */
DDE_WEAK int dev_close(struct net_device * a) {
	dde_printf("dev_close not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int dev_err(const struct device * a, const char * b, ...) {
	dde_printf("dev_err not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void * dev_get_drvdata(const struct device * a) {
	dde_printf("dev_get_drvdata not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int device_set_wakeup_enable(struct device * a, bool b) {
	dde_printf("device_set_wakeup_enable not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int _dev_info(const struct device * a, const char * b, ...) {
	dde_printf("_dev_info not implemented\n");
	return 0;
}

/* Use this variant in places where it could be invoked
 * from either hardware interrupt or other context, with hardware interrupts
 * either disabled or enabled.
 */
DDE_WEAK void dev_kfree_skb_any(struct sk_buff * a) {
	dde_printf("dev_kfree_skb_any not implemented\n");
}

/*
 */
DDE_WEAK int dev_open(struct net_device * a) {
	dde_printf("dev_open not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int dev_set_drvdata(struct device * a, void * b) {
	dde_printf("dev_set_drvdata not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int dev_warn(const struct device * a, const char * b, ...) {
	dde_printf("dev_warn not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void disable_irq(unsigned int a) {
	dde_printf("disable_irq not implemented\n");
}

/*
 */
struct dma_map_ops * dma_ops;

/*
 */
DDE_WEAK int dma_set_mask(struct device * a, u64 b) {
	dde_printf("dma_set_mask not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int dma_supported(struct device * a, u64 b) {
	dde_printf("dma_supported not implemented\n");
	return 0;
}

/* Record number of completed objects and recalculate the limit. */
DDE_WEAK void dql_completed(struct dql * a, unsigned int b) {
	dde_printf("dql_completed not implemented\n");
}

/* Reset dql state */
DDE_WEAK void dql_reset(struct dql * a) {
	dde_printf("dql_reset not implemented\n");
}

/*
 */
DDE_WEAK void dump_stack() {
	dde_printf("dump_stack not implemented\n");
}

/*
 */
DDE_WEAK void enable_irq(unsigned int a) {
	dde_printf("enable_irq not implemented\n");
}

/*
 */
DDE_WEAK int ethtool_op_get_ts_info(struct net_device * a, struct ethtool_ts_info * b) {
	dde_printf("ethtool_op_get_ts_info not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK __be16 eth_type_trans(struct sk_buff * a, struct net_device * b) {
	dde_printf("eth_type_trans not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int eth_validate_addr(struct net_device * a) {
	dde_printf("eth_validate_addr not implemented\n");
	return 0;
}

/**
 * find_first_bit - find the first set bit in a memory region
 * @addr: The address to start the search at
 * @size: The maximum size to search
 *
 * Returns the bit number of the first set bit.
 */
DDE_WEAK unsigned long find_first_bit(const unsigned long * a, unsigned long b) {
	dde_printf("find_first_bit not implemented\n");
	return 0;
}

/**
 * find_next_bit - find the next set bit in a memory region
 * @addr: The address to base the search on
 * @offset: The bitnumber to start searching at
 * @size: The bitmap size in bits
 */
DDE_WEAK unsigned long find_next_bit(const unsigned long * a, unsigned long b, unsigned long c) {
	dde_printf("find_next_bit not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void free_irq(unsigned int a, void * b) {
	dde_printf("free_irq not implemented\n");
}

/*
 */
DDE_WEAK void free_netdev(struct net_device * a) {
	dde_printf("free_netdev not implemented\n");
}

/*
 */
DDE_WEAK void init_timer_key(struct timer_list * a, unsigned int b, const char * c, struct lock_class_key * d) {
	dde_printf("init_timer_key not implemented\n");
}

/*
 */
DDE_WEAK void ioread16_rep(void * a, void * b, unsigned long c) {
	dde_printf("ioread16_rep not implemented\n");
}

/**
 * ioremap     -   map bus memory into CPU space
 * @offset:    bus address of the memory
 * @size:      size of the resource to map
 *
 * ioremap performs a platform specific sequence of operations to
 * make bus memory CPU accessible via the readb/readw/readl/writeb/
 * writew/writel functions and the other mmio helpers. The returned
 * address is not guaranteed to be usable directly as a virtual
 * address.
 *
 * If the area you are trying to map is a PCI BAR you should have a
 * look at pci_iomap().
 */
DDE_WEAK void * ioremap_nocache(resource_size_t a, unsigned long b) {
	dde_printf("ioremap_nocache not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void iounmap(volatile void * a) {
	dde_printf("iounmap not implemented\n");
}

/*
 */
DDE_WEAK void iowrite16_rep(void * a, const void * b, unsigned long c) {
	dde_printf("iowrite16_rep not implemented\n");
}

/*
 */
volatile unsigned long jiffies;

typeof(unsigned long) kernel_stack;

/*
 */
DDE_WEAK void kfree(const void * a) {
	dde_printf("kfree not implemented\n");
}

/*
 */
DDE_WEAK void * __kmalloc(size_t a, gfp_t b) {
	dde_printf("__kmalloc not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void local_bh_disable() {
	dde_printf("local_bh_disable not implemented\n");
}

/*
 */
DDE_WEAK void local_bh_enable() {
	dde_printf("local_bh_enable not implemented\n");
}

/*
 */
DDE_WEAK void * __memcpy(void * a, const void * b, size_t c) {
	dde_printf("__memcpy not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void * memset(void * a, int b, size_t c) {
	dde_printf("memset not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK ssize_t __modver_version_show(struct module_attribute * a, struct module_kobject * b, char * c) {
	dde_printf("__modver_version_show not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void msleep(unsigned int a) {
	dde_printf("msleep not implemented\n");
}

/*
 */
DDE_WEAK unsigned long msleep_interruptible(unsigned int a) {
	dde_printf("msleep_interruptible not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void __mutex_init(struct mutex * a, const char * b, struct lock_class_key * c) {
	dde_printf("__mutex_init not implemented\n");
}

/*
 */
DDE_WEAK void mutex_lock(struct mutex * a) {
	dde_printf("mutex_lock not implemented\n");
}

/*
 */
DDE_WEAK void mutex_unlock(struct mutex * a) {
	dde_printf("mutex_unlock not implemented\n");
}

/*
 */
DDE_WEAK void napi_complete(struct napi_struct * a) {
	dde_printf("napi_complete not implemented\n");
}

/*
 */
DDE_WEAK gro_result_t napi_gro_receive(struct napi_struct * a, struct sk_buff * b) {
	dde_printf("napi_gro_receive not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void __napi_schedule(struct napi_struct * a) {
	dde_printf("__napi_schedule not implemented\n");
}

/*
 */
DDE_WEAK struct sk_buff * __netdev_alloc_skb(struct net_device * a, unsigned int b, gfp_t c) {
	dde_printf("__netdev_alloc_skb not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int netdev_err(const struct net_device * a, const char * b, ...) {
	dde_printf("netdev_err not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int netdev_info(const struct net_device * a, const char * b, ...) {
	dde_printf("netdev_info not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int netdev_warn(const struct net_device * a, const char * b, ...) {
	dde_printf("netdev_warn not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void netif_carrier_off(struct net_device * a) {
	dde_printf("netif_carrier_off not implemented\n");
}

/*
 */
DDE_WEAK void netif_carrier_on(struct net_device * a) {
	dde_printf("netif_carrier_on not implemented\n");
}

/*
 */
DDE_WEAK void netif_device_attach(struct net_device * a) {
	dde_printf("netif_device_attach not implemented\n");
}

/*
 */
DDE_WEAK void netif_device_detach(struct net_device * a) {
	dde_printf("netif_device_detach not implemented\n");
}

/*
 */
DDE_WEAK void netif_napi_add(struct net_device * a, struct napi_struct * b, int (*c)(struct napi_struct *, int), int d) {
	dde_printf("netif_napi_add not implemented\n");
}

/*
 */
DDE_WEAK void __netif_schedule(struct Qdisc * a) {
	dde_printf("__netif_schedule not implemented\n");
}

/*
 */
DDE_WEAK int net_ratelimit() {
	dde_printf("net_ratelimit not implemented\n");
	return 0;
}

struct kernel_param_ops param_array_ops;

struct kernel_param_ops param_ops_int;

struct kernel_param_ops param_ops_uint;

/*
 */
DDE_WEAK int pci_bus_read_config_word(struct pci_bus * a, unsigned int b, int c, u16 * d) {
	dde_printf("pci_bus_read_config_word not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void pci_clear_mwi(struct pci_dev * a) {
	dde_printf("pci_clear_mwi not implemented\n");
}

/*
 */
DDE_WEAK void pci_disable_device(struct pci_dev * a) {
	dde_printf("pci_disable_device not implemented\n");
}

/*
 */
DDE_WEAK int pci_enable_device(struct pci_dev * a) {
	dde_printf("pci_enable_device not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int pci_enable_device_mem(struct pci_dev * a) {
	dde_printf("pci_enable_device_mem not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int __pci_enable_wake(struct pci_dev * a, pci_power_t b, bool c, bool d) {
	dde_printf("__pci_enable_wake not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void * pci_ioremap_bar(struct pci_dev * a, int b) {
	dde_printf("pci_ioremap_bar not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int pci_prepare_to_sleep(struct pci_dev * a) {
	dde_printf("pci_prepare_to_sleep not implemented\n");
	return 0;
}

/* Proper probing supporting hot-pluggable devices */
DDE_WEAK int __pci_register_driver(struct pci_driver * a, struct module * b, const char * c) {
	dde_printf("__pci_register_driver not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void pci_release_selected_regions(struct pci_dev * a, int b) {
	dde_printf("pci_release_selected_regions not implemented\n");
}

/*
 */
DDE_WEAK int pci_request_selected_regions(struct pci_dev * a, int b, const char * c) {
	dde_printf("pci_request_selected_regions not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void pci_restore_state(struct pci_dev * a) {
	dde_printf("pci_restore_state not implemented\n");
}

/* Power management related routines */
DDE_WEAK int pci_save_state(struct pci_dev * a) {
	dde_printf("pci_save_state not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int pci_select_bars(struct pci_dev * a, unsigned long b) {
	dde_printf("pci_select_bars not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void pci_set_master(struct pci_dev * a) {
	dde_printf("pci_set_master not implemented\n");
}

/*
 */
DDE_WEAK int pci_set_mwi(struct pci_dev * a) {
	dde_printf("pci_set_mwi not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int pci_set_power_state(struct pci_dev * a, pci_power_t b) {
	dde_printf("pci_set_power_state not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void pci_unregister_driver(struct pci_driver * a) {
	dde_printf("pci_unregister_driver not implemented\n");
}

/*
 */
DDE_WEAK int pci_wake_from_d3(struct pci_dev * a, bool b) {
	dde_printf("pci_wake_from_d3 not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int pcix_get_mmrbc(struct pci_dev * a) {
	dde_printf("pcix_get_mmrbc not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int pcix_set_mmrbc(struct pci_dev * a, int b) {
	dde_printf("pcix_set_mmrbc not implemented\n");
	return 0;
}

unsigned long phys_base;

/*
 */
DDE_WEAK void print_hex_dump(const char * a, const char * b, int c, int d, int e, const void * f, size_t g, bool h) {
	dde_printf("print_hex_dump not implemented\n");
}

/*
 */
DDE_WEAK int printk(const char * fmt, ...) {
	va_list ap;
	int cnt;
	va_start(ap, fmt);
	cnt = vkprintf(fmt, ap);
	va_end(ap);
	return cnt;
}

/*
 */
DDE_WEAK int pskb_expand_head(struct sk_buff * a, int b, int c, gfp_t d) {
	dde_printf("pskb_expand_head not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK unsigned char * __pskb_pull_tail(struct sk_buff * a, int b) {
	dde_printf("__pskb_pull_tail not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int ___pskb_trim(struct sk_buff * a, unsigned int b) {
	dde_printf("___pskb_trim not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void put_page(struct page * a) {
	dde_printf("put_page not implemented\n");
}

/*
 */
DDE_WEAK bool queue_delayed_work_on(int a, struct workqueue_struct * b, struct delayed_work * c, unsigned long d) {
	dde_printf("queue_delayed_work_on not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK bool queue_work_on(int a, struct workqueue_struct * b, struct work_struct * c) {
	dde_printf("queue_work_on not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void _raw_spin_lock(raw_spinlock_t * a) {
	dde_printf("_raw_spin_lock not implemented\n");
}

/*
 */
DDE_WEAK unsigned long _raw_spin_lock_irqsave(raw_spinlock_t * a) {
	dde_printf("_raw_spin_lock_irqsave not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void _raw_spin_unlock_irqrestore(raw_spinlock_t * a, unsigned long b) {
	dde_printf("_raw_spin_unlock_irqrestore not implemented\n");
}

/*
 */
DDE_WEAK int register_netdev(struct net_device * a) {
	dde_printf("register_netdev not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int request_threaded_irq(unsigned int a, irq_handler_t b, irq_handler_t c, unsigned long d, const char * e, void * f) {
	dde_printf("request_threaded_irq not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int rtnl_is_locked() {
	dde_printf("rtnl_is_locked not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK int skb_pad(struct sk_buff * a, int b) {
	dde_printf("skb_pad not implemented\n");
	return 0;
}

/*
 *	Add data to an sk_buff
 */
DDE_WEAK unsigned char * skb_put(struct sk_buff * a, unsigned int b) {
	dde_printf("skb_put not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void skb_trim(struct sk_buff * a, unsigned int b) {
	dde_printf("skb_trim not implemented\n");
}

/**
 * skb_tstamp_tx - queue clone of skb with send time stamps
 * @orig_skb:	the original outgoing packet
 * @hwtstamps:	hardware time stamps, may be NULL if not available
 *
 * If the skb has a socket associated, then this function clones the
 * skb (thus sharing the actual data and optional structures), stores
 * the optional hardware time stamping information (if non NULL) or
 * generates a software time stamp (otherwise), then queues the clone
 * to the error queue of the socket.  Errors are silently ignored.
 */
DDE_WEAK void skb_tstamp_tx(struct sk_buff * a, struct skb_shared_hwtstamps * b) {
	dde_printf("skb_tstamp_tx not implemented\n");
}

/*
 */
DDE_WEAK char * strcpy(char * a, const char * b) {
	dde_printf("strcpy not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK unsigned long strlcpy(char * a, const char * b, size_t c) {
	dde_printf("strlcpy not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK char * strncpy(char * a, const char * b, __kernel_size_t c) {
	dde_printf("strncpy not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void synchronize_irq(unsigned int a) {
	dde_printf("synchronize_irq not implemented\n");
}

/*
 */
enum system_states system_state;

/*
 */
struct workqueue_struct * system_wq;

/*
 */
DDE_WEAK void __udelay(unsigned long a) {
	dde_printf("__udelay not implemented\n");
}

/*
 */
DDE_WEAK void unregister_netdev(struct net_device * a) {
	dde_printf("unregister_netdev not implemented\n");
}

/*
 */
DDE_WEAK void vfree(const void * a) {
	dde_printf("vfree not implemented\n");
}

/*
 */
DDE_WEAK void * vzalloc(unsigned long a) {
	dde_printf("vzalloc not implemented\n");
	return 0;
}

/*
 */
DDE_WEAK void warn_slowpath_null(const char * a, const int b) {
	dde_printf("warn_slowpath_null not implemented\n");
}

/*
 */
struct device x86_dma_fallback_dev;

