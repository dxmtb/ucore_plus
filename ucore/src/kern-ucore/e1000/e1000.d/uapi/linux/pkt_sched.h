#ifndef __UAPI_LINUX_PKT_SCHED_H__
#define __UAPI_LINUX_PKT_SCHED_H__

#include <linux/types.h>

#define TC_PRIO_MAX			15

struct tc_stats {
	__u64	bytes;			/* Number of enqueued bytes */
	__u32	packets;		/* Number of enqueued packets	*/
	__u32	drops;			/* Packets dropped because of lack of resources */
	__u32	overlimits;		/* Number of throttle events when this
					 * flow goes out of allocated bandwidth */
	__u32	bps;			/* Current flow byte rate */
	__u32	pps;			/* Current flow packet rate */
	__u32	qlen;
	__u32	backlog;
};

#define TC_LINKLAYER_MASK 0x0F /* limit use to lower 4 bits */

struct tc_sizespec {
	unsigned char	cell_log;
	unsigned char	size_log;
	short		cell_align;
	int		overhead;
	unsigned int	linklayer;
	unsigned int	mpu;
	unsigned int	mtu;
	unsigned int	tsize;
};

#define TC_QOPT_BITMASK 15
#define TC_QOPT_MAX_QUEUE 16

#endif /* ! __UAPI_LINUX_PKT_SCHED_H__ */
