#ifndef __UAPI_LINUX_GEN_STATS_H__
#define __UAPI_LINUX_GEN_STATS_H__

#include <linux/types.h>

struct gnet_stats_basic_packed {
	__u64	bytes;
	__u32	packets;
} __attribute__ ((packed));

/**
 * struct gnet_stats_rate_est - rate estimator
 * @bps: current byte rate
 * @pps: current packet rate
 */
struct gnet_stats_rate_est {
	__u32	bps;
	__u32	pps;
};

/**
 * struct gnet_stats_queue - queuing statistics
 * @qlen: queue length
 * @backlog: backlog size of queue
 * @drops: number of dropped packets
 * @requeues: number of requeues
 * @overlimits: number of enqueues over the limit
 */
struct gnet_stats_queue {
	__u32	qlen;
	__u32	backlog;
	__u32	drops;
	__u32	requeues;
	__u32	overlimits;
};

#endif /* ! __UAPI_LINUX_GEN_STATS_H__ */
