#ifndef __NET_GEN_STATS_H__
#define __NET_GEN_STATS_H__

#include <linux/gen_stats.h>
#include <linux/socket.h>
#include <linux/rtnetlink.h>
#include <linux/pkt_sched.h>

struct gnet_dump {
	spinlock_t *      lock;
	struct sk_buff *  skb;
	struct nlattr *   tail;

	/* Backward compatibility */
	int               compat_tc_stats;
	int               compat_xstats;
	void *            xstats;
	int               xstats_len;
	struct tc_stats   tc_stats;
};

#endif /* ! __NET_GEN_STATS_H__ */
