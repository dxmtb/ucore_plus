#ifndef __LINUX_NETDEVICE_H__
#define __LINUX_NETDEVICE_H__

#include <linux/pm_qos.h>
#include <linux/timer.h>
#include <linux/bug.h>
#include <linux/delay.h>
#include <linux/atomic.h>
#include <asm/cache.h>
#include <asm/byteorder.h>

#include <linux/percpu.h>
#include <linux/rculist.h>
#include <linux/dmaengine.h>
#include <linux/workqueue.h>
#include <linux/dynamic_queue_limits.h>

#include <linux/ethtool.h>
#include <net/net_namespace.h>
#include <net/dsa.h>

#include <net/netprio_cgroup.h>

#include <linux/netdev_features.h>
#include <linux/neighbour.h>
#include <uapi/linux/netdevice.h>

struct netpoll_info;
struct device;
struct phy_device;

struct wireless_dev;

#define SET_ETHTOOL_OPS(netdev,ops) \
	( (netdev)->ethtool_ops = (ops) )

#define NET_ADDR_RANDOM		1	/* address is generated randomly */

#define NET_XMIT_SUCCESS	0x00
#define NET_XMIT_DROP		0x01	/* skb dropped			*/

#define NET_XMIT_MASK		0x0f	/* qdisc flags in net/sch_generic.h */

enum netdev_tx {
	__NETDEV_TX_MIN	 = INT_MIN,	/* make sure enum is signed */
	NETDEV_TX_OK	 = 0x00,	/* driver took care of packet */
	NETDEV_TX_BUSY	 = 0x10,	/* driver tx path was busy*/
	NETDEV_TX_LOCKED = 0x20,	/* driver tx lock was already taken */
};
typedef enum netdev_tx netdev_tx_t;

#  define LL_MAX_HEADER 96

struct net_device_stats {
	unsigned long	rx_packets;
	unsigned long	tx_packets;
	unsigned long	rx_bytes;
	unsigned long	tx_bytes;
	unsigned long	rx_errors;
	unsigned long	tx_errors;
	unsigned long	rx_dropped;
	unsigned long	tx_dropped;
	unsigned long	multicast;
	unsigned long	collisions;
	unsigned long	rx_length_errors;
	unsigned long	rx_over_errors;
	unsigned long	rx_crc_errors;
	unsigned long	rx_frame_errors;
	unsigned long	rx_fifo_errors;
	unsigned long	rx_missed_errors;
	unsigned long	tx_aborted_errors;
	unsigned long	tx_carrier_errors;
	unsigned long	tx_fifo_errors;
	unsigned long	tx_heartbeat_errors;
	unsigned long	tx_window_errors;
	unsigned long	rx_compressed;
	unsigned long	tx_compressed;
};

#include <linux/cache.h>
#include <linux/skbuff.h>

#include <linux/static_key.h>

struct neighbour;
struct neigh_parms;
struct sk_buff;

struct netdev_hw_addr {
	struct list_head	list;
	unsigned char		addr[MAX_ADDR_LEN];
	unsigned char		type;
#define NETDEV_HW_ADDR_T_LAN		1
#define NETDEV_HW_ADDR_T_SAN		2
#define NETDEV_HW_ADDR_T_SLAVE		3
#define NETDEV_HW_ADDR_T_UNICAST	4
#define NETDEV_HW_ADDR_T_MULTICAST	5
	bool			global_use;
	int			sync_cnt;
	int			refcount;
	int			synced;
	struct rcu_head		rcu_head;
};

struct netdev_hw_addr_list {
	struct list_head	list;
	int			count;
};

#define netdev_hw_addr_list_count(l) ((l)->count)

#define netdev_hw_addr_list_for_each(ha, l) \
	list_for_each_entry(ha, &(l)->list, list)

#define netdev_uc_count(dev) netdev_hw_addr_list_count(&(dev)->uc)

#define netdev_for_each_uc_addr(ha, dev) \
	netdev_hw_addr_list_for_each(ha, &(dev)->uc)

#define netdev_for_each_mc_addr(ha, dev) \
	netdev_hw_addr_list_for_each(ha, &(dev)->mc)

struct hh_cache {
	u16		hh_len;
	u16		__pad;
	seqlock_t	hh_lock;

	/* cached hardware header; allow for machine alignment needs.        */
#define HH_DATA_MOD	16
#define HH_DATA_OFF(__len) \
	(HH_DATA_MOD - (((__len - 1) & (HH_DATA_MOD - 1)) + 1))
#define HH_DATA_ALIGN(__len) \
	(((__len)+(HH_DATA_MOD-1))&~(HH_DATA_MOD - 1))
	unsigned long	hh_data[HH_DATA_ALIGN(LL_MAX_HEADER) / sizeof(long)];
};

struct header_ops {
	int	(*create) (struct sk_buff *skb, struct net_device *dev,
			   unsigned short type, const void *daddr,
			   const void *saddr, unsigned int len);
	int	(*parse)(const struct sk_buff *skb, unsigned char *haddr);
	int	(*rebuild)(struct sk_buff *skb);
	int	(*cache)(const struct neighbour *neigh, struct hh_cache *hh, __be16 type);
	void	(*cache_update)(struct hh_cache *hh,
				const struct net_device *dev,
				const unsigned char *haddr);
};

enum netdev_state_t {
	__LINK_STATE_START,
	__LINK_STATE_PRESENT,
	__LINK_STATE_NOCARRIER,
	__LINK_STATE_LINKWATCH_PENDING,
	__LINK_STATE_DORMANT,
};

/*
 * Structure for NAPI scheduling similar to tasklet but with weighting
 */
struct napi_struct {
	/* The poll_list must only be managed by the entity which
	 * changes the state of the NAPI_STATE_SCHED bit.  This means
	 * whoever atomically sets that bit can add this napi_struct
	 * to the per-cpu poll_list, and whoever clears that bit
	 * can remove from the list right before clearing the bit.
	 */
	struct list_head	poll_list;

	unsigned long		state;
	int			weight;
	unsigned int		gro_count;
	int			(*poll)(struct napi_struct *, int);
#ifdef CONFIG_NETPOLL
	spinlock_t		poll_lock;
	int			poll_owner;
#endif
	struct net_device	*dev;
	struct sk_buff		*gro_list;
	struct sk_buff		*skb;
	struct list_head	dev_list;
};

enum {
	NAPI_STATE_SCHED,	/* Poll is scheduled */
	NAPI_STATE_DISABLE,	/* Disable pending */
	NAPI_STATE_NPSVC,	/* Netpoll - don't dequeue from poll_list */
};

enum gro_result {
	GRO_MERGED,
	GRO_MERGED_FREE,
	GRO_HELD,
	GRO_NORMAL,
	GRO_DROP,
};
typedef enum gro_result gro_result_t;

enum rx_handler_result {
	RX_HANDLER_CONSUMED,
	RX_HANDLER_ANOTHER,
	RX_HANDLER_EXACT,
	RX_HANDLER_PASS,
};
typedef enum rx_handler_result rx_handler_result_t;
typedef rx_handler_result_t rx_handler_func_t(struct sk_buff **pskb);

extern void __napi_schedule(struct napi_struct *n);

static inline bool napi_disable_pending(struct napi_struct *n)
{
	return test_bit(NAPI_STATE_DISABLE, &n->state);
}

/**
 *	napi_schedule_prep - check if napi can be scheduled
 *	@n: napi context
 *
 * Test if NAPI routine is already running, and if not mark
 * it as running.  This is used as a condition variable
 * insure only one NAPI poll instance runs.  We also make
 * sure there is no pending NAPI disable.
 */
static inline bool napi_schedule_prep(struct napi_struct *n)
{
	return !napi_disable_pending(n) &&
		!test_and_set_bit(NAPI_STATE_SCHED, &n->state);
}

extern void napi_complete(struct napi_struct *n);

/**
 *	napi_disable - prevent NAPI from scheduling
 *	@n: napi context
 *
 * Stop NAPI from being scheduled on this context.
 * Waits till any outstanding processing completes.
 */
static inline void napi_disable(struct napi_struct *n)
{
	set_bit(NAPI_STATE_DISABLE, &n->state);
	while (test_and_set_bit(NAPI_STATE_SCHED, &n->state))
		msleep(1);
	clear_bit(NAPI_STATE_DISABLE, &n->state);
}

/**
 *	napi_enable - enable NAPI scheduling
 *	@n: napi context
 *
 * Resume NAPI from being scheduled on this context.
 * Must be paired with napi_disable.
 */
static inline void napi_enable(struct napi_struct *n)
{
	BUG_ON(!test_bit(NAPI_STATE_SCHED, &n->state));
	smp_mb__before_clear_bit();
	clear_bit(NAPI_STATE_SCHED, &n->state);
}

enum netdev_queue_state_t {
	__QUEUE_STATE_DRV_XOFF,
	__QUEUE_STATE_STACK_XOFF,
	__QUEUE_STATE_FROZEN,
#define QUEUE_STATE_ANY_XOFF ((1 << __QUEUE_STATE_DRV_XOFF)		| \
			      (1 << __QUEUE_STATE_STACK_XOFF))
#define QUEUE_STATE_ANY_XOFF_OR_FROZEN (QUEUE_STATE_ANY_XOFF		| \
					(1 << __QUEUE_STATE_FROZEN))
};

struct netdev_queue {
/*
 * read mostly part
 */
	struct net_device	*dev;
	struct Qdisc		*qdisc;
	struct Qdisc		*qdisc_sleeping;
#ifdef CONFIG_SYSFS
	struct kobject		kobj;
#endif
#if defined(CONFIG_XPS) && defined(CONFIG_NUMA)
	int			numa_node;
#endif
/*
 * write mostly part
 */
	spinlock_t		_xmit_lock ____cacheline_aligned_in_smp;
	int			xmit_lock_owner;
	/*
	 * please use this field instead of dev->trans_start
	 */
	unsigned long		trans_start;

	/*
	 * Number of TX timeouts for this queue
	 * (/sys/class/net/DEV/Q/trans_timeout)
	 */
	unsigned long		trans_timeout;

	unsigned long		state;

#ifdef CONFIG_BQL
	struct dql		dql;
#endif
} ____cacheline_aligned_in_smp;

/*
 * This structure holds an RPS map which can be of variable length.  The
 * map is an array of CPUs.
 */
struct rps_map {
	unsigned int len;
	struct rcu_head rcu;
	u16 cpus[0];
};

/*
 * The rps_dev_flow structure contains the mapping of a flow to a CPU, the
 * tail pointer for that CPU's input queue at the time of last enqueue, and
 * a hardware filter index.
 */
struct rps_dev_flow {
	u16 cpu;
	u16 filter;
	unsigned int last_qtail;
};

/*
 * The rps_dev_flow_table structure contains a table of flow mappings.
 */
struct rps_dev_flow_table {
	unsigned int mask;
	struct rcu_head rcu;
	struct rps_dev_flow flows[0];
};

/*
 * The rps_sock_flow_table contains mappings of flows to the last CPU
 * on which they were processed by the application (set in recvmsg).
 */
struct rps_sock_flow_table {
	unsigned int mask;
	u16 ents[0];
};

#define RPS_NO_CPU 0xffff

static inline void rps_record_sock_flow(struct rps_sock_flow_table *table,
					u32 hash)
{
	if (table && hash) {
		unsigned int cpu, index = hash & table->mask;

		/* We only give a hint, preemption can change cpu under us */
		cpu = raw_smp_processor_id();

		if (table->ents[index] != cpu)
			table->ents[index] = cpu;
	}
}

static inline void rps_reset_sock_flow(struct rps_sock_flow_table *table,
				       u32 hash)
{
	if (table && hash)
		table->ents[hash & table->mask] = RPS_NO_CPU;
}

extern struct rps_sock_flow_table __rcu *rps_sock_flow_table;

struct netdev_rx_queue {
	struct rps_map __rcu		*rps_map;
	struct rps_dev_flow_table __rcu	*rps_flow_table;
	struct kobject			kobj;
	struct net_device		*dev;
} ____cacheline_aligned_in_smp;

/*
 * This structure holds an XPS map which can be of variable length.  The
 * map is an array of queues.
 */
struct xps_map {
	unsigned int len;
	unsigned int alloc_len;
	struct rcu_head rcu;
	u16 queues[0];
};

/*
 * This structure holds all XPS maps for device.  Maps are indexed by CPU.
 */
struct xps_dev_maps {
	struct rcu_head rcu;
	struct xps_map __rcu *cpu_map[0];
};

#define TC_MAX_QUEUE	16
#define TC_BITMASK	15

struct netdev_tc_txq {
	u16 count;
	u16 offset;
};

/*
 * This structure defines the management hooks for network devices.
 * The following hooks can be defined; unless noted otherwise, they are
 * optional and can be filled with a null pointer.
 *
 * int (*ndo_init)(struct net_device *dev);
 *     This function is called once when network device is registered.
 *     The network device can use this to any late stage initializaton
 *     or semantic validattion. It can fail with an error code which will
 *     be propogated back to register_netdev
 *
 * void (*ndo_uninit)(struct net_device *dev);
 *     This function is called when device is unregistered or when registration
 *     fails. It is not called if init fails.
 *
 * int (*ndo_open)(struct net_device *dev);
 *     This function is called when network device transistions to the up
 *     state.
 *
 * int (*ndo_stop)(struct net_device *dev);
 *     This function is called when network device transistions to the down
 *     state.
 *
 * netdev_tx_t (*ndo_start_xmit)(struct sk_buff *skb,
 *                               struct net_device *dev);
 *	Called when a packet needs to be transmitted.
 *	Must return NETDEV_TX_OK , NETDEV_TX_BUSY.
 *        (can also return NETDEV_TX_LOCKED iff NETIF_F_LLTX)
 *	Required can not be NULL.
 *
 * u16 (*ndo_select_queue)(struct net_device *dev, struct sk_buff *skb);
 *	Called to decide which queue to when device supports multiple
 *	transmit queues.
 *
 * void (*ndo_change_rx_flags)(struct net_device *dev, int flags);
 *	This function is called to allow device receiver to make
 *	changes to configuration when multicast or promiscious is enabled.
 *
 * void (*ndo_set_rx_mode)(struct net_device *dev);
 *	This function is called device changes address list filtering.
 *	If driver handles unicast address filtering, it should set
 *	IFF_UNICAST_FLT to its priv_flags.
 *
 * int (*ndo_set_mac_address)(struct net_device *dev, void *addr);
 *	This function  is called when the Media Access Control address
 *	needs to be changed. If this interface is not defined, the
 *	mac address can not be changed.
 *
 * int (*ndo_validate_addr)(struct net_device *dev);
 *	Test if Media Access Control address is valid for the device.
 *
 * int (*ndo_do_ioctl)(struct net_device *dev, struct ifreq *ifr, int cmd);
 *	Called when a user request an ioctl which can't be handled by
 *	the generic interface code. If not defined ioctl's return
 *	not supported error code.
 *
 * int (*ndo_set_config)(struct net_device *dev, struct ifmap *map);
 *	Used to set network devices bus interface parameters. This interface
 *	is retained for legacy reason, new devices should use the bus
 *	interface (PCI) for low level management.
 *
 * int (*ndo_change_mtu)(struct net_device *dev, int new_mtu);
 *	Called when a user wants to change the Maximum Transfer Unit
 *	of a device. If not defined, any request to change MTU will
 *	will return an error.
 *
 * void (*ndo_tx_timeout)(struct net_device *dev);
 *	Callback uses when the transmitter has not made any progress
 *	for dev->watchdog ticks.
 *
 * struct rtnl_link_stats64* (*ndo_get_stats64)(struct net_device *dev,
 *                      struct rtnl_link_stats64 *storage);
 * struct net_device_stats* (*ndo_get_stats)(struct net_device *dev);
 *	Called when a user wants to get the network device usage
 *	statistics. Drivers must do one of the following:
 *	1. Define @ndo_get_stats64 to fill in a zero-initialised
 *	   rtnl_link_stats64 structure passed by the caller.
 *	2. Define @ndo_get_stats to update a net_device_stats structure
 *	   (which should normally be dev->stats) and return a pointer to
 *	   it. The structure may be changed asynchronously only if each
 *	   field is written atomically.
 *	3. Update dev->stats asynchronously and atomically, and define
 *	   neither operation.
 *
 * int (*ndo_vlan_rx_add_vid)(struct net_device *dev, __be16 proto, u16t vid);
 *	If device support VLAN filtering this function is called when a
 *	VLAN id is registered.
 *
 * int (*ndo_vlan_rx_kill_vid)(struct net_device *dev, unsigned short vid);
 *	If device support VLAN filtering this function is called when a
 *	VLAN id is unregistered.
 *
 * void (*ndo_poll_controller)(struct net_device *dev);
 *
 *	SR-IOV management functions.
 * int (*ndo_set_vf_mac)(struct net_device *dev, int vf, u8* mac);
 * int (*ndo_set_vf_vlan)(struct net_device *dev, int vf, u16 vlan, u8 qos);
 * int (*ndo_set_vf_tx_rate)(struct net_device *dev, int vf, int rate);
 * int (*ndo_set_vf_spoofchk)(struct net_device *dev, int vf, bool setting);
 * int (*ndo_get_vf_config)(struct net_device *dev,
 *			    int vf, struct ifla_vf_info *ivf);
 * int (*ndo_set_vf_port)(struct net_device *dev, int vf,
 *			  struct nlattr *port[]);
 * int (*ndo_get_vf_port)(struct net_device *dev, int vf, struct sk_buff *skb);
 * int (*ndo_setup_tc)(struct net_device *dev, u8 tc)
 * 	Called to setup 'tc' number of traffic classes in the net device. This
 * 	is always called from the stack with the rtnl lock held and netif tx
 * 	queues stopped. This allows the netdevice to perform queue management
 * 	safely.
 *
 *	Fiber Channel over Ethernet (FCoE) offload functions.
 * int (*ndo_fcoe_enable)(struct net_device *dev);
 *	Called when the FCoE protocol stack wants to start using LLD for FCoE
 *	so the underlying device can perform whatever needed configuration or
 *	initialization to support acceleration of FCoE traffic.
 *
 * int (*ndo_fcoe_disable)(struct net_device *dev);
 *	Called when the FCoE protocol stack wants to stop using LLD for FCoE
 *	so the underlying device can perform whatever needed clean-ups to
 *	stop supporting acceleration of FCoE traffic.
 *
 * int (*ndo_fcoe_ddp_setup)(struct net_device *dev, u16 xid,
 *			     struct scatterlist *sgl, unsigned int sgc);
 *	Called when the FCoE Initiator wants to initialize an I/O that
 *	is a possible candidate for Direct Data Placement (DDP). The LLD can
 *	perform necessary setup and returns 1 to indicate the device is set up
 *	successfully to perform DDP on this I/O, otherwise this returns 0.
 *
 * int (*ndo_fcoe_ddp_done)(struct net_device *dev,  u16 xid);
 *	Called when the FCoE Initiator/Target is done with the DDPed I/O as
 *	indicated by the FC exchange id 'xid', so the underlying device can
 *	clean up and reuse resources for later DDP requests.
 *
 * int (*ndo_fcoe_ddp_target)(struct net_device *dev, u16 xid,
 *			      struct scatterlist *sgl, unsigned int sgc);
 *	Called when the FCoE Target wants to initialize an I/O that
 *	is a possible candidate for Direct Data Placement (DDP). The LLD can
 *	perform necessary setup and returns 1 to indicate the device is set up
 *	successfully to perform DDP on this I/O, otherwise this returns 0.
 *
 * int (*ndo_fcoe_get_hbainfo)(struct net_device *dev,
 *			       struct netdev_fcoe_hbainfo *hbainfo);
 *	Called when the FCoE Protocol stack wants information on the underlying
 *	device. This information is utilized by the FCoE protocol stack to
 *	register attributes with Fiber Channel management service as per the
 *	FC-GS Fabric Device Management Information(FDMI) specification.
 *
 * int (*ndo_fcoe_get_wwn)(struct net_device *dev, u64 *wwn, int type);
 *	Called when the underlying device wants to override default World Wide
 *	Name (WWN) generation mechanism in FCoE protocol stack to pass its own
 *	World Wide Port Name (WWPN) or World Wide Node Name (WWNN) to the FCoE
 *	protocol stack to use.
 *
 *	RFS acceleration.
 * int (*ndo_rx_flow_steer)(struct net_device *dev, const struct sk_buff *skb,
 *			    u16 rxq_index, u32 flow_id);
 *	Set hardware filter for RFS.  rxq_index is the target queue index;
 *	flow_id is a flow ID to be passed to rps_may_expire_flow() later.
 *	Return the filter ID on success, or a negative error code.
 *
 *	Slave management functions (for bridge, bonding, etc).
 * int (*ndo_add_slave)(struct net_device *dev, struct net_device *slave_dev);
 *	Called to make another netdev an underling.
 *
 * int (*ndo_del_slave)(struct net_device *dev, struct net_device *slave_dev);
 *	Called to release previously enslaved netdev.
 *
 *      Feature/offload setting functions.
 * netdev_features_t (*ndo_fix_features)(struct net_device *dev,
 *		netdev_features_t features);
 *	Adjusts the requested feature flags according to device-specific
 *	constraints, and returns the resulting flags. Must not modify
 *	the device state.
 *
 * int (*ndo_set_features)(struct net_device *dev, netdev_features_t features);
 *	Called to update device configuration to new features. Passed
 *	feature set might be less than what was returned by ndo_fix_features()).
 *	Must return >0 or -errno if it changed dev->features itself.
 *
 * int (*ndo_fdb_add)(struct ndmsg *ndm, struct nlattr *tb[],
 *		      struct net_device *dev,
 *		      const unsigned char *addr, u16 flags)
 *	Adds an FDB entry to dev for addr.
 * int (*ndo_fdb_del)(struct ndmsg *ndm, struct nlattr *tb[],
 *		      struct net_device *dev,
 *		      const unsigned char *addr)
 *	Deletes the FDB entry from dev coresponding to addr.
 * int (*ndo_fdb_dump)(struct sk_buff *skb, struct netlink_callback *cb,
 *		       struct net_device *dev, int idx)
 *	Used to add FDB entries to dump requests. Implementers should add
 *	entries to skb and update idx with the number of entries.
 *
 * int (*ndo_bridge_setlink)(struct net_device *dev, struct nlmsghdr *nlh)
 * int (*ndo_bridge_getlink)(struct sk_buff *skb, u32 pid, u32 seq,
 *			     struct net_device *dev, u32 filter_mask)
 *
 * int (*ndo_change_carrier)(struct net_device *dev, bool new_carrier);
 *	Called to change device carrier. Soft-devices (like dummy, team, etc)
 *	which do not represent real hardware may define this to allow their
 *	userspace components to manage their virtual carrier state. Devices
 *	that determine carrier state from physical hardware properties (eg
 *	network cables) or protocol-dependent mechanisms (eg
 *	USB_CDC_NOTIFY_NETWORK_CONNECTION) should NOT implement this function.
 */
struct net_device_ops {
	int			(*ndo_init)(struct net_device *dev);
	void			(*ndo_uninit)(struct net_device *dev);
	int			(*ndo_open)(struct net_device *dev);
	int			(*ndo_stop)(struct net_device *dev);
	netdev_tx_t		(*ndo_start_xmit) (struct sk_buff *skb,
						   struct net_device *dev);
	u16			(*ndo_select_queue)(struct net_device *dev,
						    struct sk_buff *skb);
	void			(*ndo_change_rx_flags)(struct net_device *dev,
						       int flags);
	void			(*ndo_set_rx_mode)(struct net_device *dev);
	int			(*ndo_set_mac_address)(struct net_device *dev,
						       void *addr);
	int			(*ndo_validate_addr)(struct net_device *dev);
	int			(*ndo_do_ioctl)(struct net_device *dev,
					        struct ifreq *ifr, int cmd);
	int			(*ndo_set_config)(struct net_device *dev,
					          struct ifmap *map);
	int			(*ndo_change_mtu)(struct net_device *dev,
						  int new_mtu);
	int			(*ndo_neigh_setup)(struct net_device *dev,
						   struct neigh_parms *);
	void			(*ndo_tx_timeout) (struct net_device *dev);

	struct rtnl_link_stats64* (*ndo_get_stats64)(struct net_device *dev,
						     struct rtnl_link_stats64 *storage);
	struct net_device_stats* (*ndo_get_stats)(struct net_device *dev);

	int			(*ndo_vlan_rx_add_vid)(struct net_device *dev,
						       __be16 proto, u16 vid);
	int			(*ndo_vlan_rx_kill_vid)(struct net_device *dev,
						        __be16 proto, u16 vid);
#ifdef CONFIG_NET_POLL_CONTROLLER
	void                    (*ndo_poll_controller)(struct net_device *dev);
	int			(*ndo_netpoll_setup)(struct net_device *dev,
						     struct netpoll_info *info,
						     gfp_t gfp);
	void			(*ndo_netpoll_cleanup)(struct net_device *dev);
#endif
	int			(*ndo_set_vf_mac)(struct net_device *dev,
						  int queue, u8 *mac);
	int			(*ndo_set_vf_vlan)(struct net_device *dev,
						   int queue, u16 vlan, u8 qos);
	int			(*ndo_set_vf_tx_rate)(struct net_device *dev,
						      int vf, int rate);
	int			(*ndo_set_vf_spoofchk)(struct net_device *dev,
						       int vf, bool setting);
	int			(*ndo_get_vf_config)(struct net_device *dev,
						     int vf,
						     struct ifla_vf_info *ivf);
	int			(*ndo_set_vf_port)(struct net_device *dev,
						   int vf,
						   struct nlattr *port[]);
	int			(*ndo_get_vf_port)(struct net_device *dev,
						   int vf, struct sk_buff *skb);
	int			(*ndo_setup_tc)(struct net_device *dev, u8 tc);
#if IS_ENABLED(CONFIG_FCOE)
	int			(*ndo_fcoe_enable)(struct net_device *dev);
	int			(*ndo_fcoe_disable)(struct net_device *dev);
	int			(*ndo_fcoe_ddp_setup)(struct net_device *dev,
						      u16 xid,
						      struct scatterlist *sgl,
						      unsigned int sgc);
	int			(*ndo_fcoe_ddp_done)(struct net_device *dev,
						     u16 xid);
	int			(*ndo_fcoe_ddp_target)(struct net_device *dev,
						       u16 xid,
						       struct scatterlist *sgl,
						       unsigned int sgc);
	int			(*ndo_fcoe_get_hbainfo)(struct net_device *dev,
							struct netdev_fcoe_hbainfo *hbainfo);
#endif

#if IS_ENABLED(CONFIG_LIBFCOE)
#define NETDEV_FCOE_WWNN 0
#define NETDEV_FCOE_WWPN 1
	int			(*ndo_fcoe_get_wwn)(struct net_device *dev,
						    u64 *wwn, int type);
#endif

#ifdef CONFIG_RFS_ACCEL
	int			(*ndo_rx_flow_steer)(struct net_device *dev,
						     const struct sk_buff *skb,
						     u16 rxq_index,
						     u32 flow_id);
#endif
	int			(*ndo_add_slave)(struct net_device *dev,
						 struct net_device *slave_dev);
	int			(*ndo_del_slave)(struct net_device *dev,
						 struct net_device *slave_dev);
	netdev_features_t	(*ndo_fix_features)(struct net_device *dev,
						    netdev_features_t features);
	int			(*ndo_set_features)(struct net_device *dev,
						    netdev_features_t features);
	int			(*ndo_neigh_construct)(struct neighbour *n);
	void			(*ndo_neigh_destroy)(struct neighbour *n);

	int			(*ndo_fdb_add)(struct ndmsg *ndm,
					       struct nlattr *tb[],
					       struct net_device *dev,
					       const unsigned char *addr,
					       u16 flags);
	int			(*ndo_fdb_del)(struct ndmsg *ndm,
					       struct nlattr *tb[],
					       struct net_device *dev,
					       const unsigned char *addr);
	int			(*ndo_fdb_dump)(struct sk_buff *skb,
						struct netlink_callback *cb,
						struct net_device *dev,
						int idx);

	int			(*ndo_bridge_setlink)(struct net_device *dev,
						      struct nlmsghdr *nlh);
	int			(*ndo_bridge_getlink)(struct sk_buff *skb,
						      u32 pid, u32 seq,
						      struct net_device *dev,
						      u32 filter_mask);
	int			(*ndo_bridge_dellink)(struct net_device *dev,
						      struct nlmsghdr *nlh);
	int			(*ndo_change_carrier)(struct net_device *dev,
						      bool new_carrier);
};

struct net_device {

	/*
	 * This is the first field of the "visible" part of this structure
	 * (i.e. as seen by users in the "Space.c" file).  It is the name
	 * of the interface.
	 */
	char			name[IFNAMSIZ];

	/* device name hash chain, please keep it close to name[] */
	struct hlist_node	name_hlist;

	/* snmp alias */
	char 			*ifalias;

	/*
	 *	I/O specific fields
	 *	FIXME: Merge these and struct ifmap into one
	 */
	unsigned long		mem_end;	/* shared mem end	*/
	unsigned long		mem_start;	/* shared mem start	*/
	unsigned long		base_addr;	/* device I/O address	*/
	unsigned int		irq;		/* device IRQ number	*/

	/*
	 *	Some hardware also needs these fields, but they are not
	 *	part of the usual set specified in Space.c.
	 */

	unsigned long		state;

	struct list_head	dev_list;
	struct list_head	napi_list;
	struct list_head	unreg_list;
	struct list_head	upper_dev_list; /* List of upper devices */


	/* currently active device features */
	netdev_features_t	features;
	/* user-changeable features */
	netdev_features_t	hw_features;
	/* user-requested features */
	netdev_features_t	wanted_features;
	/* mask of features inheritable by VLAN devices */
	netdev_features_t	vlan_features;
	/* mask of features inherited by encapsulating devices
	 * This field indicates what encapsulation offloads
	 * the hardware is capable of doing, and drivers will
	 * need to set them appropriately.
	 */
	netdev_features_t	hw_enc_features;

	/* Interface index. Unique device identifier	*/
	int			ifindex;
	int			iflink;

	struct net_device_stats	stats;
	atomic_long_t		rx_dropped; /* dropped packets by core network
					     * Do not use this in drivers.
					     */

#ifdef CONFIG_WIRELESS_EXT
	/* List of functions to handle Wireless Extensions (instead of ioctl).
	 * See <net/iw_handler.h> for details. Jean II */
	const struct iw_handler_def *	wireless_handlers;
	/* Instance data managed by the core of Wireless Extensions. */
	struct iw_public_data *	wireless_data;
#endif
	/* Management operations */
	const struct net_device_ops *netdev_ops;
	const struct ethtool_ops *ethtool_ops;

	/* Hardware header description */
	const struct header_ops *header_ops;

	unsigned int		flags;	/* interface flags (a la BSD)	*/
	unsigned int		priv_flags; /* Like 'flags' but invisible to userspace.
					     * See if.h for definitions. */
	unsigned short		gflags;
	unsigned short		padded;	/* How much padding added by alloc_netdev() */

	unsigned char		operstate; /* RFC2863 operstate */
	unsigned char		link_mode; /* mapping policy to operstate */

	unsigned char		if_port;	/* Selectable AUI, TP,..*/
	unsigned char		dma;		/* DMA channel		*/

	unsigned int		mtu;	/* interface MTU value		*/
	unsigned short		type;	/* interface hardware type	*/
	unsigned short		hard_header_len;	/* hardware hdr length	*/

	/* extra head- and tailroom the hardware may need, but not in all cases
	 * can this be guaranteed, especially tailroom. Some cases also use
	 * LL_MAX_HEADER instead to allocate the skb.
	 */
	unsigned short		needed_headroom;
	unsigned short		needed_tailroom;

	/* Interface address info. */
	unsigned char		perm_addr[MAX_ADDR_LEN]; /* permanent hw address */
	unsigned char		addr_assign_type; /* hw address assignment type */
	unsigned char		addr_len;	/* hardware address length	*/
	unsigned char		neigh_priv_len;
	unsigned short          dev_id;		/* for shared network cards */

	spinlock_t		addr_list_lock;
	struct netdev_hw_addr_list	uc;	/* Unicast mac addresses */
	struct netdev_hw_addr_list	mc;	/* Multicast mac addresses */
	struct netdev_hw_addr_list	dev_addrs; /* list of device
						    * hw addresses
						    */
#ifdef CONFIG_SYSFS
	struct kset		*queues_kset;
#endif

	bool			uc_promisc;
	unsigned int		promiscuity;
	unsigned int		allmulti;


	/* Protocol specific pointers */

#if IS_ENABLED(CONFIG_VLAN_8021Q)
	struct vlan_info __rcu	*vlan_info;	/* VLAN info */
#endif
#if IS_ENABLED(CONFIG_NET_DSA)
	struct dsa_switch_tree	*dsa_ptr;	/* dsa specific data */
#endif
	void 			*atalk_ptr;	/* AppleTalk link 	*/
	struct in_device __rcu	*ip_ptr;	/* IPv4 specific data	*/
	struct dn_dev __rcu     *dn_ptr;        /* DECnet specific data */
	struct inet6_dev __rcu	*ip6_ptr;       /* IPv6 specific data */
	void			*ax25_ptr;	/* AX.25 specific data */
	struct wireless_dev	*ieee80211_ptr;	/* IEEE 802.11 specific data,
						   assign before registering */

/*
 * Cache lines mostly used on receive path (including eth_type_trans())
 */
	unsigned long		last_rx;	/* Time of last Rx
						 * This should not be set in
						 * drivers, unless really needed,
						 * because network stack (bonding)
						 * use it if/when necessary, to
						 * avoid dirtying this cache line.
						 */

	/* Interface address info used in eth_type_trans() */
	unsigned char		*dev_addr;	/* hw address, (before bcast
						   because most packets are
						   unicast) */


#ifdef CONFIG_RPS
	struct netdev_rx_queue	*_rx;

	/* Number of RX queues allocated at register_netdev() time */
	unsigned int		num_rx_queues;

	/* Number of RX queues currently active in device */
	unsigned int		real_num_rx_queues;

#endif

	rx_handler_func_t __rcu	*rx_handler;
	void __rcu		*rx_handler_data;

	struct netdev_queue __rcu *ingress_queue;
	unsigned char		broadcast[MAX_ADDR_LEN];	/* hw bcast add	*/


/*
 * Cache lines mostly used on transmit path
 */
	struct netdev_queue	*_tx ____cacheline_aligned_in_smp;

	/* Number of TX queues allocated at alloc_netdev_mq() time  */
	unsigned int		num_tx_queues;

	/* Number of TX queues currently active in device  */
	unsigned int		real_num_tx_queues;

	/* root qdisc from userspace point of view */
	struct Qdisc		*qdisc;

	unsigned long		tx_queue_len;	/* Max frames per queue allowed */
	spinlock_t		tx_global_lock;

#ifdef CONFIG_XPS
	struct xps_dev_maps __rcu *xps_maps;
#endif
#ifdef CONFIG_RFS_ACCEL
	/* CPU reverse-mapping for RX completion interrupts, indexed
	 * by RX queue number.  Assigned by driver.  This must only be
	 * set if the ndo_rx_flow_steer operation is defined. */
	struct cpu_rmap		*rx_cpu_rmap;
#endif

	/* These may be needed for future network-power-down code. */

	/*
	 * trans_start here is expensive for high speed devices on SMP,
	 * please use netdev_queue->trans_start instead.
	 */
	unsigned long		trans_start;	/* Time (in jiffies) of last Tx	*/

	int			watchdog_timeo; /* used by dev_watchdog() */
	struct timer_list	watchdog_timer;

	/* Number of references to this device */
	int __percpu		*pcpu_refcnt;

	/* delayed register/unregister */
	struct list_head	todo_list;
	/* device index hash chain */
	struct hlist_node	index_hlist;

	struct list_head	link_watch_list;

	/* register/unregister state machine */
	enum { NETREG_UNINITIALIZED=0,
	       NETREG_REGISTERED,	/* completed register_netdevice */
	       NETREG_UNREGISTERING,	/* called unregister_netdevice */
	       NETREG_UNREGISTERED,	/* completed unregister todo */
	       NETREG_RELEASED,		/* called free_netdev */
	       NETREG_DUMMY,		/* dummy device for NAPI poll */
	} reg_state:8;

	bool dismantle; /* device is going do be freed */

	enum {
		RTNL_LINK_INITIALIZED,
		RTNL_LINK_INITIALIZING,
	} rtnl_link_state:16;

	/* Called from unregister, can be used to call free_netdev */
	void (*destructor)(struct net_device *dev);

#ifdef CONFIG_NETPOLL
	struct netpoll_info __rcu	*npinfo;
#endif

#ifdef CONFIG_NET_NS
	/* Network namespace this network device is inside */
	struct net		*nd_net;
#endif

	/* mid-layer private */
	union {
		void				*ml_priv;
		struct pcpu_lstats __percpu	*lstats; /* loopback stats */
		struct pcpu_tstats __percpu	*tstats; /* tunnel stats */
		struct pcpu_dstats __percpu	*dstats; /* dummy stats */
		struct pcpu_vstats __percpu	*vstats; /* veth stats */
	};
	/* GARP */
	struct garp_port __rcu	*garp_port;
	/* MRP */
	struct mrp_port __rcu	*mrp_port;

	/* class/net/name entry */
	struct device		dev;
	/* space for optional device, statistics, and wireless sysfs groups */
	const struct attribute_group *sysfs_groups[4];

	/* rtnetlink link ops */
	const struct rtnl_link_ops *rtnl_link_ops;

	/* for setting kernel sock attribute on TCP connection setup */
#define GSO_MAX_SIZE		65536
	unsigned int		gso_max_size;
#define GSO_MAX_SEGS		65535
	u16			gso_max_segs;

#ifdef CONFIG_DCB
	/* Data Center Bridging netlink ops */
	const struct dcbnl_rtnl_ops *dcbnl_ops;
#endif
	u8 num_tc;
	struct netdev_tc_txq tc_to_txq[TC_MAX_QUEUE];
	u8 prio_tc_map[TC_BITMASK + 1];

#if IS_ENABLED(CONFIG_FCOE)
	/* max exchange id for FCoE LRO by ddp */
	unsigned int		fcoe_ddp_xid;
#endif
#if IS_ENABLED(CONFIG_NETPRIO_CGROUP)
	struct netprio_map __rcu *priomap;
#endif
	/* phy device may attach itself for hardware timestamping */
	struct phy_device *phydev;

	struct lock_class_key *qdisc_tx_busylock;

	/* group the device belongs to */
	int group;

	struct pm_qos_request	pm_qos_req;
};

#define	NETDEV_ALIGN		32

static inline
struct netdev_queue *netdev_get_tx_queue(const struct net_device *dev,
					 unsigned int index)
{
	return &dev->_tx[index];
}

/*
 * Net namespace inlines
 */
static inline
struct net *dev_net(const struct net_device *dev)
{
	return read_pnet(&dev->nd_net);
}

/**
 *	netdev_priv - access network device private data
 *	@dev: network device
 *
 * Get network device private data
 */
static inline void *netdev_priv(const struct net_device *dev)
{
	return (char *)dev + ALIGN(sizeof(struct net_device), NETDEV_ALIGN);
}

/* Set the sysfs physical device reference for the network logical device
 * if set prior to registration will cause a symlink during initialization.
 */
#define SET_NETDEV_DEV(net, pdev)	((net)->dev.parent = (pdev))

/**
 *	netif_napi_add - initialize a napi context
 *	@dev:  network device
 *	@napi: napi context
 *	@poll: polling function
 *	@weight: default weight
 *
 * netif_napi_add() must be used to initialize a napi context prior to calling
 * *any* of the other napi related functions.
 */
void netif_napi_add(struct net_device *dev, struct napi_struct *napi,
		    int (*poll)(struct napi_struct *, int), int weight);

#define NAPI_GRO_CB(skb) ((struct napi_gro_cb *)(skb)->cb)

#include <linux/notifier.h>

#define net_device_entry(lh)	list_entry(lh, struct net_device, dev_list)

extern int		dev_open(struct net_device *dev);
extern int		dev_close(struct net_device *dev);

extern int		dev_queue_xmit(struct sk_buff *skb);

extern void		unregister_netdevice_queue(struct net_device *dev,
						   struct list_head *head);

extern void		free_netdev(struct net_device *dev);

typedef int gifconf_func_t(struct net_device * dev, char __user * bufptr, int len);
extern int		register_gifconf(unsigned int family, gifconf_func_t * gifconf);

extern void __netif_schedule(struct Qdisc *q);

static inline void netif_schedule_queue(struct netdev_queue *txq)
{
	if (!(txq->state & QUEUE_STATE_ANY_XOFF))
		__netif_schedule(txq->qdisc);
}

static inline void netif_tx_start_queue(struct netdev_queue *dev_queue)
{
	clear_bit(__QUEUE_STATE_DRV_XOFF, &dev_queue->state);
}

/**
 *	netif_start_queue - allow transmit
 *	@dev: network device
 *
 *	Allow upper layers to call the device hard_start_xmit routine.
 */
static inline void netif_start_queue(struct net_device *dev)
{
	netif_tx_start_queue(netdev_get_tx_queue(dev, 0));
}

static inline void netif_tx_wake_queue(struct netdev_queue *dev_queue)
{
#ifdef CONFIG_NETPOLL_TRAP
	if (netpoll_trap()) {
		netif_tx_start_queue(dev_queue);
		return;
	}
#endif
	if (test_and_clear_bit(__QUEUE_STATE_DRV_XOFF, &dev_queue->state))
		__netif_schedule(dev_queue->qdisc);
}

/**
 *	netif_wake_queue - restart transmit
 *	@dev: network device
 *
 *	Allow upper layers to call the device hard_start_xmit routine.
 *	Used for flow control when transmit resources are available.
 */
static inline void netif_wake_queue(struct net_device *dev)
{
	netif_tx_wake_queue(netdev_get_tx_queue(dev, 0));
}

static inline void netif_tx_stop_queue(struct netdev_queue *dev_queue)
{
	if (WARN_ON(!dev_queue)) {
		pr_info("netif_stop_queue() cannot be called before register_netdev()\n");
		return;
	}
	set_bit(__QUEUE_STATE_DRV_XOFF, &dev_queue->state);
}

/**
 *	netif_stop_queue - stop transmitted packets
 *	@dev: network device
 *
 *	Stop upper layers calling the device hard_start_xmit routine.
 *	Used for flow control when transmit resources are unavailable.
 */
static inline void netif_stop_queue(struct net_device *dev)
{
	netif_tx_stop_queue(netdev_get_tx_queue(dev, 0));
}

static inline bool netif_tx_queue_stopped(const struct netdev_queue *dev_queue)
{
	return test_bit(__QUEUE_STATE_DRV_XOFF, &dev_queue->state);
}

/**
 *	netif_queue_stopped - test if transmit queue is flowblocked
 *	@dev: network device
 *
 *	Test if transmit queue on device is currently unable to send.
 */
static inline bool netif_queue_stopped(const struct net_device *dev)
{
	return netif_tx_queue_stopped(netdev_get_tx_queue(dev, 0));
}

static inline void netdev_tx_sent_queue(struct netdev_queue *dev_queue,
					unsigned int bytes)
{
#ifdef CONFIG_BQL
	dql_queued(&dev_queue->dql, bytes);

	if (likely(dql_avail(&dev_queue->dql) >= 0))
		return;

	set_bit(__QUEUE_STATE_STACK_XOFF, &dev_queue->state);

	/*
	 * The XOFF flag must be set before checking the dql_avail below,
	 * because in netdev_tx_completed_queue we update the dql_completed
	 * before checking the XOFF flag.
	 */
	smp_mb();

	/* check again in case another CPU has just made room avail */
	if (unlikely(dql_avail(&dev_queue->dql) >= 0))
		clear_bit(__QUEUE_STATE_STACK_XOFF, &dev_queue->state);
#endif
}

static inline void netdev_sent_queue(struct net_device *dev, unsigned int bytes)
{
	netdev_tx_sent_queue(netdev_get_tx_queue(dev, 0), bytes);
}

static inline void netdev_tx_completed_queue(struct netdev_queue *dev_queue,
					     unsigned int pkts, unsigned int bytes)
{
#ifdef CONFIG_BQL
	if (unlikely(!bytes))
		return;

	dql_completed(&dev_queue->dql, bytes);

	/*
	 * Without the memory barrier there is a small possiblity that
	 * netdev_tx_sent_queue will miss the update and cause the queue to
	 * be stopped forever
	 */
	smp_mb();

	if (dql_avail(&dev_queue->dql) < 0)
		return;

	if (test_and_clear_bit(__QUEUE_STATE_STACK_XOFF, &dev_queue->state))
		netif_schedule_queue(dev_queue);
#endif
}

static inline void netdev_completed_queue(struct net_device *dev,
					  unsigned int pkts, unsigned int bytes)
{
	netdev_tx_completed_queue(netdev_get_tx_queue(dev, 0), pkts, bytes);
}

static inline void netdev_tx_reset_queue(struct netdev_queue *q)
{
#ifdef CONFIG_BQL
	clear_bit(__QUEUE_STATE_STACK_XOFF, &q->state);
	dql_reset(&q->dql);
#endif
}

static inline void netdev_reset_queue(struct net_device *dev_queue)
{
	netdev_tx_reset_queue(netdev_get_tx_queue(dev_queue, 0));
}

/**
 *	netif_running - test if up
 *	@dev: network device
 *
 *	Test if the device has been brought up.
 */
static inline bool netif_running(const struct net_device *dev)
{
	return test_bit(__LINK_STATE_START, &dev->state);
}

/**
 *	netif_subqueue_stopped - test status of subqueue
 *	@dev: network device
 *	@queue_index: sub queue index
 *
 * Check individual transmit queue of a device with multiple transmit queues.
 */
static inline bool __netif_subqueue_stopped(const struct net_device *dev,
					    u16 queue_index)
{
	struct netdev_queue *txq = netdev_get_tx_queue(dev, queue_index);

	return netif_tx_queue_stopped(txq);
}

extern int netif_set_real_num_tx_queues(struct net_device *dev,
					unsigned int txq);

extern int netif_set_real_num_rx_queues(struct net_device *dev,
					unsigned int rxq);

/* Use this variant in places where it could be invoked
 * from either hardware interrupt or other context, with hardware interrupts
 * either disabled or enabled.
 */
extern void dev_kfree_skb_any(struct sk_buff *skb);

extern gro_result_t	napi_gro_receive(struct napi_struct *napi,
					 struct sk_buff *skb);

extern void linkwatch_fire_event(struct net_device *dev);

/**
 *	netif_carrier_ok - test if carrier present
 *	@dev: network device
 *
 * Check if carrier is present on device
 */
static inline bool netif_carrier_ok(const struct net_device *dev)
{
	return !test_bit(__LINK_STATE_NOCARRIER, &dev->state);
}

extern void netif_carrier_on(struct net_device *dev);

extern void netif_carrier_off(struct net_device *dev);

extern void netif_device_detach(struct net_device *dev);

extern void netif_device_attach(struct net_device *dev);

enum {
	NETIF_MSG_DRV		= 0x0001,
	NETIF_MSG_PROBE		= 0x0002,
	NETIF_MSG_LINK		= 0x0004,
	NETIF_MSG_TIMER		= 0x0008,
	NETIF_MSG_IFDOWN	= 0x0010,
	NETIF_MSG_IFUP		= 0x0020,
	NETIF_MSG_RX_ERR	= 0x0040,
	NETIF_MSG_TX_ERR	= 0x0080,
	NETIF_MSG_TX_QUEUED	= 0x0100,
	NETIF_MSG_INTR		= 0x0200,
	NETIF_MSG_TX_DONE	= 0x0400,
	NETIF_MSG_RX_STATUS	= 0x0800,
	NETIF_MSG_PKTDATA	= 0x1000,
	NETIF_MSG_HW		= 0x2000,
	NETIF_MSG_WOL		= 0x4000,
};

#define netif_msg_drv(p)	((p)->msg_enable & NETIF_MSG_DRV)
#define netif_msg_probe(p)	((p)->msg_enable & NETIF_MSG_PROBE)

#define netif_msg_rx_err(p)	((p)->msg_enable & NETIF_MSG_RX_ERR)
#define netif_msg_tx_err(p)	((p)->msg_enable & NETIF_MSG_TX_ERR)

#define netif_msg_tx_done(p)	((p)->msg_enable & NETIF_MSG_TX_DONE)
#define netif_msg_rx_status(p)	((p)->msg_enable & NETIF_MSG_RX_STATUS)

#define netif_msg_hw(p)		((p)->msg_enable & NETIF_MSG_HW)

static inline u32 netif_msg_init(int debug_value, int default_msg_enable_bits)
{
	return (1 << debug_value) - 1;
	/* use default */
	if (debug_value < 0 || debug_value >= (sizeof(u32) * 8))
		return default_msg_enable_bits;
	if (debug_value == 0)	/* no output */
		return 0;
	/* set low N bits */
	return (1 << debug_value) - 1;
}

static inline void __netif_tx_lock(struct netdev_queue *txq, int cpu)
{
	spin_lock(&txq->_xmit_lock);
	txq->xmit_lock_owner = cpu;
}

static inline void __netif_tx_unlock(struct netdev_queue *txq)
{
	txq->xmit_lock_owner = -1;
	spin_unlock(&txq->_xmit_lock);
}

/**
 *	netif_tx_lock - grab network device transmit lock
 *	@dev: network device
 *
 * Get network device transmit lock
 */
static inline void netif_tx_lock(struct net_device *dev)
{
	unsigned int i;
	int cpu;

	spin_lock(&dev->tx_global_lock);
	cpu = smp_processor_id();
	for (i = 0; i < dev->num_tx_queues; i++) {
		struct netdev_queue *txq = netdev_get_tx_queue(dev, i);

		/* We are the only thread of execution doing a
		 * freeze, but we have to grab the _xmit_lock in
		 * order to synchronize with threads which are in
		 * the ->hard_start_xmit() handler and already
		 * checked the frozen bit.
		 */
		__netif_tx_lock(txq, cpu);
		set_bit(__QUEUE_STATE_FROZEN, &txq->state);
		__netif_tx_unlock(txq);
	}
}

static inline void netif_tx_unlock(struct net_device *dev)
{
	unsigned int i;

	for (i = 0; i < dev->num_tx_queues; i++) {
		struct netdev_queue *txq = netdev_get_tx_queue(dev, i);

		/* No need to grab the _xmit_lock here.  If the
		 * queue is not stopped for another reason, we
		 * force a schedule.
		 */
		clear_bit(__QUEUE_STATE_FROZEN, &txq->state);
		netif_schedule_queue(txq);
	}
	spin_unlock(&dev->tx_global_lock);
}

static inline void netif_tx_disable(struct net_device *dev)
{
	unsigned int i;
	int cpu;

	local_bh_disable();
	cpu = smp_processor_id();
	for (i = 0; i < dev->num_tx_queues; i++) {
		struct netdev_queue *txq = netdev_get_tx_queue(dev, i);

		__netif_tx_lock(txq, cpu);
		netif_tx_stop_queue(txq);
		__netif_tx_unlock(txq);
	}
	local_bh_enable();
}

/*
 * dev_addrs walker. Should be used only for read access. Call with
 * rcu_read_lock held.
 */
#define for_each_dev_addr(dev, ha) \
		list_for_each_entry_rcu(ha, &dev->dev_addrs.list, list)

extern int		register_netdev(struct net_device *dev);
extern void		unregister_netdev(struct net_device *dev);

extern struct sk_buff *__skb_gso_segment(struct sk_buff *skb,
	netdev_features_t features, bool tx_path);

netdev_features_t netdev_increment_features(netdev_features_t all,
	netdev_features_t one, netdev_features_t mask);

static inline bool net_gso_ok(netdev_features_t features, int gso_type)
{
	netdev_features_t feature = gso_type << NETIF_F_GSO_SHIFT;

	/* check flags correspondence */
	BUILD_BUG_ON(SKB_GSO_TCPV4   != (NETIF_F_TSO >> NETIF_F_GSO_SHIFT));
	BUILD_BUG_ON(SKB_GSO_UDP     != (NETIF_F_UFO >> NETIF_F_GSO_SHIFT));
	BUILD_BUG_ON(SKB_GSO_DODGY   != (NETIF_F_GSO_ROBUST >> NETIF_F_GSO_SHIFT));
	BUILD_BUG_ON(SKB_GSO_TCP_ECN != (NETIF_F_TSO_ECN >> NETIF_F_GSO_SHIFT));
	BUILD_BUG_ON(SKB_GSO_TCPV6   != (NETIF_F_TSO6 >> NETIF_F_GSO_SHIFT));
	BUILD_BUG_ON(SKB_GSO_FCOE    != (NETIF_F_FSO >> NETIF_F_GSO_SHIFT));

	return (features & feature) == feature;
}

static inline bool skb_gso_ok(struct sk_buff *skb, netdev_features_t features)
{
	return net_gso_ok(features, skb_shinfo(skb)->gso_type) &&
	       (!skb_has_frag_list(skb) || (features & NETIF_F_FRAGLIST));
}

extern __printf(3, 4)
int netdev_printk(const char *level, const struct net_device *dev,
		  const char *format, ...);

extern __printf(2, 3)
int netdev_err(const struct net_device *dev, const char *format, ...);
extern __printf(2, 3)
int netdev_warn(const struct net_device *dev, const char *format, ...);

extern __printf(2, 3)
int netdev_info(const struct net_device *dev, const char *format, ...);

#define netdev_dbg(__dev, format, args...)			\
({								\
	if (0)							\
		netdev_printk(KERN_DEBUG, __dev, format, ##args); \
	0;							\
})

#define netif_level(level, priv, type, dev, fmt, args...)	\
do {								\
	if (netif_msg_##type(priv))				\
		netdev_##level(dev, fmt, ##args);		\
} while (0)

#define netif_err(priv, type, dev, fmt, args...)		\
	netif_level(err, priv, type, dev, fmt, ##args)
#define netif_warn(priv, type, dev, fmt, args...)		\
	netif_level(warn, priv, type, dev, fmt, ##args)

#define netif_info(priv, type, dev, fmt, args...)		\
	netif_level(info, priv, type, dev, fmt, ##args)

#endif /* ! __LINUX_NETDEVICE_H__ */
