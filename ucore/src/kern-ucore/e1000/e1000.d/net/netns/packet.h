#ifndef __NET_NETNS_PACKET_H__
#define __NET_NETNS_PACKET_H__

#include <linux/rculist.h>
#include <linux/mutex.h>

struct netns_packet {
	struct mutex		sklist_lock;
	struct hlist_head	sklist;
};

#endif /* ! __NET_NETNS_PACKET_H__ */
