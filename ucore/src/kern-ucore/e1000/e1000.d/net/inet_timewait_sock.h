#ifndef __NET_INET_TIMEWAIT_SOCK_H__
#define __NET_INET_TIMEWAIT_SOCK_H__

#include <linux/kmemcheck.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <linux/workqueue.h>

#include <net/inet_sock.h>
#include <net/sock.h>
#include <net/tcp_states.h>
#include <net/timewait_sock.h>

#include <linux/atomic.h>

struct inet_hashinfo;

#define INET_TWDR_RECYCLE_SLOTS_LOG	5
#define INET_TWDR_RECYCLE_SLOTS		(1 << INET_TWDR_RECYCLE_SLOTS_LOG)

#define INET_TWDR_TWKILL_SLOTS	8 /* Please keep this a power of 2. */

struct inet_bind_bucket;

/*
 * This is a TIME_WAIT sock. It works around the memory consumption
 * problems of sockets in such a state on heavily loaded servers, but
 * without violating the protocol specification.
 */
struct inet_timewait_sock {
	/*
	 * Now struct sock also uses sock_common, so please just
	 * don't add nothing before this first member (__tw_common) --acme
	 */
	struct sock_common	__tw_common;
#define tw_family		__tw_common.skc_family
#define tw_state		__tw_common.skc_state
#define tw_reuse		__tw_common.skc_reuse
#define tw_bound_dev_if		__tw_common.skc_bound_dev_if
#define tw_node			__tw_common.skc_nulls_node
#define tw_bind_node		__tw_common.skc_bind_node
#define tw_refcnt		__tw_common.skc_refcnt
#define tw_hash			__tw_common.skc_hash
#define tw_prot			__tw_common.skc_prot
#define tw_net			__tw_common.skc_net
#define tw_daddr        	__tw_common.skc_daddr
#define tw_rcv_saddr    	__tw_common.skc_rcv_saddr
#define tw_addrpair		__tw_common.skc_addrpair
#define tw_dport		__tw_common.skc_dport
#define tw_num			__tw_common.skc_num
#define tw_portpair		__tw_common.skc_portpair

	int			tw_timeout;
	volatile unsigned char	tw_substate;
	unsigned char		tw_rcv_wscale;

	/* Socket demultiplex comparisons on incoming packets. */
	/* these three are in inet_sock */
	__be16			tw_sport;
	kmemcheck_bitfield_begin(flags);
	/* And these are ours. */
	unsigned int		tw_ipv6only     : 1,
				tw_transparent  : 1,
				tw_pad		: 6,	/* 6 bits hole */
				tw_tos		: 8,
				tw_ipv6_offset  : 16;
	kmemcheck_bitfield_end(flags);
	unsigned long		tw_ttd;
	struct inet_bind_bucket	*tw_tb;
	struct hlist_node	tw_death_node;
};

static inline int inet_twsk_dead_hashed(const struct inet_timewait_sock *tw)
{
	return !hlist_unhashed(&tw->tw_death_node);
}

static inline void inet_twsk_dead_node_init(struct inet_timewait_sock *tw)
{
	tw->tw_death_node.pprev = NULL;
}

static inline void __inet_twsk_del_dead_node(struct inet_timewait_sock *tw)
{
	__hlist_del(&tw->tw_death_node);
	inet_twsk_dead_node_init(tw);
}

static inline struct inet_timewait_sock *inet_twsk(const struct sock *sk)
{
	return (struct inet_timewait_sock *)sk;
}

#endif /* ! __NET_INET_TIMEWAIT_SOCK_H__ */
