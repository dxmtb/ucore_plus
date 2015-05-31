#ifndef __UAPI_LINUX_IPV6_H__
#define __UAPI_LINUX_IPV6_H__

#include <linux/types.h>
#include <linux/in6.h>
#include <asm/byteorder.h>

struct in6_pktinfo {
	struct in6_addr	ipi6_addr;
	int		ipi6_ifindex;
};

struct ipv6hdr {
#if defined(__LITTLE_ENDIAN_BITFIELD)
	__u8			priority:4,
				version:4;
#elif defined(__BIG_ENDIAN_BITFIELD)
	__u8			version:4,
				priority:4;
#else
#error	"Please fix <asm/byteorder.h>"
#endif
	__u8			flow_lbl[3];

	__be16			payload_len;
	__u8			nexthdr;
	__u8			hop_limit;

	struct	in6_addr	saddr;
	struct	in6_addr	daddr;
};

#endif /* ! __UAPI_LINUX_IPV6_H__ */
