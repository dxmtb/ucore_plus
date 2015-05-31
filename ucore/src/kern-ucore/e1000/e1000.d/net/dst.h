#ifndef __NET_DST_H__
#define __NET_DST_H__

#include <net/dst_ops.h>
#include <linux/netdevice.h>
#include <linux/rtnetlink.h>
#include <linux/rcupdate.h>
#include <linux/bug.h>
#include <linux/jiffies.h>
#include <net/neighbour.h>
#include <asm/processor.h>

struct sk_buff;

struct dst_entry {
	struct rcu_head		rcu_head;
	struct dst_entry	*child;
	struct net_device       *dev;
	struct  dst_ops	        *ops;
	unsigned long		_metrics;
	unsigned long           expires;
	struct dst_entry	*path;
	struct dst_entry	*from;
#ifdef CONFIG_XFRM
	struct xfrm_state	*xfrm;
#else
	void			*__pad1;
#endif
	int			(*input)(struct sk_buff *);
	int			(*output)(struct sk_buff *);

	unsigned short		flags;
#define DST_HOST		0x0001
#define DST_NOXFRM		0x0002
#define DST_NOPOLICY		0x0004
#define DST_NOHASH		0x0008
#define DST_NOCACHE		0x0010
#define DST_NOCOUNT		0x0020
#define DST_NOPEER		0x0040
#define DST_FAKE_RTABLE		0x0080
#define DST_XFRM_TUNNEL		0x0100
#define DST_XFRM_QUEUE		0x0200

	unsigned short		pending_confirm;

	short			error;

	/* A non-zero value of dst->obsolete forces by-hand validation
	 * of the route entry.  Positive values are set by the generic
	 * dst layer to indicate that the entry has been forcefully
	 * destroyed.
	 *
	 * Negative values are used by the implementation layer code to
	 * force invocation of the dst_ops->check() method.
	 */
	short			obsolete;
#define DST_OBSOLETE_NONE	0
#define DST_OBSOLETE_DEAD	2
#define DST_OBSOLETE_FORCE_CHK	-1
#define DST_OBSOLETE_KILL	-2
	unsigned short		header_len;	/* more space at head required */
	unsigned short		trailer_len;	/* space to reserve at tail */
#ifdef CONFIG_IP_ROUTE_CLASSID
	__u32			tclassid;
#else
	__u32			__pad2;
#endif

	/*
	 * Align __refcnt to a 64 bytes alignment
	 * (L1_CACHE_SIZE would be too much)
	 */
#ifdef CONFIG_64BIT
	long			__pad_to_align_refcnt[2];
#endif
	/*
	 * __refcnt wants to be on a different cache line from
	 * input/output/ops or performance tanks badly
	 */
	atomic_t		__refcnt;	/* client references	*/
	int			__use;
	unsigned long		lastuse;
	union {
		struct dst_entry	*next;
		struct rtable __rcu	*rt_next;
		struct rt6_info		*rt6_next;
		struct dn_route __rcu	*dn_next;
	};
};

#define DST_METRICS_READ_ONLY	0x1UL
#define __DST_METRICS_PTR(Y)	\
	((u32 *)((Y) & ~DST_METRICS_READ_ONLY))
#define DST_METRICS_PTR(X)	__DST_METRICS_PTR((X)->_metrics)

extern void __dst_destroy_metrics_generic(struct dst_entry *dst, unsigned long old);

static inline u32 *dst_metrics_write_ptr(struct dst_entry *dst)
{
	unsigned long p = dst->_metrics;

	BUG_ON(!p);

	if (p & DST_METRICS_READ_ONLY)
		return dst->ops->cow_metrics(dst, p);
	return __DST_METRICS_PTR(p);
}

static inline u32
dst_metric_raw(const struct dst_entry *dst, const int metric)
{
	u32 *p = DST_METRICS_PTR(dst);

	return p[metric-1];
}

static inline u32
dst_metric(const struct dst_entry *dst, const int metric)
{
	WARN_ON_ONCE(metric == RTAX_HOPLIMIT ||
		     metric == RTAX_ADVMSS ||
		     metric == RTAX_MTU);
	return dst_metric_raw(dst, metric);
}

static inline u32
dst_feature(const struct dst_entry *dst, u32 feature)
{
	return dst_metric(dst, RTAX_FEATURES) & feature;
}

static inline int
dst_metric_locked(const struct dst_entry *dst, int metric)
{
	return dst_metric(dst, RTAX_LOCK) & (1<<metric);
}

static inline void dst_hold(struct dst_entry *dst)
{
	/*
	 * If your kernel compilation stops here, please check
	 * __pad_to_align_refcnt declaration in struct dst_entry
	 */
//FIXME
	//BUILD_BUG_ON(offsetof(struct dst_entry, __refcnt) & 63);
	atomic_inc(&dst->__refcnt);
}

static inline struct dst_entry *dst_clone(struct dst_entry *dst)
{
	if (dst)
		atomic_inc(&dst->__refcnt);
	return dst;
}

extern void dst_release(struct dst_entry *dst);

static inline void refdst_drop(unsigned long refdst)
{
	if (!(refdst & SKB_DST_NOREF))
		dst_release((struct dst_entry *)(refdst & SKB_DST_PTRMASK));
}

/**
 * skb_dst_drop - drops skb dst
 * @skb: buffer
 *
 * Drops dst reference count if a reference was taken.
 */
static inline void skb_dst_drop(struct sk_buff *skb)
{
	if (skb->_skb_refdst) {
		refdst_drop(skb->_skb_refdst);
		skb->_skb_refdst = 0UL;
	}
}

/**
 * skb_dst_force - makes sure skb dst is refcounted
 * @skb: buffer
 *
 * If dst is not yet refcounted, let's do it
 */
static inline void skb_dst_force(struct sk_buff *skb)
{
	if (skb_dst_is_noref(skb)) {
		WARN_ON(!rcu_read_lock_held());
		skb->_skb_refdst &= ~SKB_DST_NOREF;
		dst_clone(skb_dst(skb));
	}
}

/**
 *	__skb_tunnel_rx - prepare skb for rx reinsert
 *	@skb: buffer
 *	@dev: tunnel device
 *
 *	After decapsulation, packet is going to re-enter (netif_rx()) our stack,
 *	so make some cleanups. (no accounting done)
 */
static inline void __skb_tunnel_rx(struct sk_buff *skb, struct net_device *dev)
{
	skb->dev = dev;

	/*
	 * Clear rxhash so that we can recalulate the hash for the
	 * encapsulated packet, unless we have already determine the hash
	 * over the L4 4-tuple.
	 */
	if (!skb->l4_rxhash)
		skb->rxhash = 0;
	skb_set_queue_mapping(skb, 0);
	skb_dst_drop(skb);
	nf_reset(skb);
}

extern void __dst_free(struct dst_entry *dst);
extern struct dst_entry *dst_destroy(struct dst_entry *dst);

static inline void dst_free(struct dst_entry *dst)
{
	if (dst->obsolete > 0)
		return;
	if (!atomic_read(&dst->__refcnt)) {
		dst = dst_destroy(dst);
		if (!dst)
			return;
	}
	__dst_free(dst);
}

struct flowi;

#endif /* ! __NET_DST_H__ */
