#ifndef __LINUX_IP_H__
#define __LINUX_IP_H__

#include <linux/skbuff.h>
#include <uapi/linux/ip.h>

static inline struct iphdr *ip_hdr(const struct sk_buff *skb)
{
	return (struct iphdr *)skb_network_header(skb);
}

#endif /* ! __LINUX_IP_H__ */
