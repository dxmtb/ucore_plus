#ifndef __UAPI_LINUX_NEIGHBOUR_H__
#define __UAPI_LINUX_NEIGHBOUR_H__

#include <linux/types.h>
#include <linux/netlink.h>

struct ndmsg {
	__u8		ndm_family;
	__u8		ndm_pad1;
	__u16		ndm_pad2;
	__s32		ndm_ifindex;
	__u16		ndm_state;
	__u8		ndm_flags;
	__u8		ndm_type;
};

#define NUD_REACHABLE	0x02

#define NUD_DELAY	0x08
#define NUD_PROBE	0x10

#define NUD_NOARP	0x40
#define NUD_PERMANENT	0x80

#endif /* ! __UAPI_LINUX_NEIGHBOUR_H__ */
