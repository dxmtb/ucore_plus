#ifndef __UAPI_LINUX_NETFILTER_H__
#define __UAPI_LINUX_NETFILTER_H__

#include <linux/types.h>
#include <linux/compiler.h>
#include <linux/sysctl.h>

#define NF_VERDICT_QBITS 16

enum {
	NFPROTO_UNSPEC =  0,
	NFPROTO_IPV4   =  2,
	NFPROTO_ARP    =  3,
	NFPROTO_BRIDGE =  7,
	NFPROTO_IPV6   = 10,
	NFPROTO_DECNET = 12,
	NFPROTO_NUMPROTO,
};

#endif /* ! __UAPI_LINUX_NETFILTER_H__ */
