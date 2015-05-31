#ifndef __NET_SNMP_H__
#define __NET_SNMP_H__

#include <linux/cache.h>
#include <linux/snmp.h>
#include <linux/smp.h>

/*
 * We use unsigned longs for most mibs but u64 for ipstats.
 */
#include <linux/u64_stats_sync.h>

#define IPSTATS_MIB_MAX	__IPSTATS_MIB_MAX
struct ipstats_mib {
	/* mibs[] must be first field of struct ipstats_mib */
	u64		mibs[IPSTATS_MIB_MAX];
	struct u64_stats_sync syncp;
};

#define ICMP_MIB_MAX	__ICMP_MIB_MAX
struct icmp_mib {
	unsigned long	mibs[ICMP_MIB_MAX];
};

#define ICMPMSG_MIB_MAX	__ICMPMSG_MIB_MAX
struct icmpmsg_mib {
	atomic_long_t	mibs[ICMPMSG_MIB_MAX];
};

#define ICMP6_MIB_MAX	__ICMP6_MIB_MAX

struct icmpv6_mib {
	unsigned long	mibs[ICMP6_MIB_MAX];
};

#define ICMP6MSG_MIB_MAX  __ICMP6MSG_MIB_MAX

struct icmpv6msg_mib {
	atomic_long_t	mibs[ICMP6MSG_MIB_MAX];
};

#define TCP_MIB_MAX	__TCP_MIB_MAX
struct tcp_mib {
	unsigned long	mibs[TCP_MIB_MAX];
};

#define UDP_MIB_MAX	__UDP_MIB_MAX
struct udp_mib {
	unsigned long	mibs[UDP_MIB_MAX];
};

#define LINUX_MIB_MAX	__LINUX_MIB_MAX
struct linux_mib {
	unsigned long	mibs[LINUX_MIB_MAX];
};

#define LINUX_MIB_XFRMMAX	__LINUX_MIB_XFRMMAX

#define SNMP_ARRAY_SZ 1

#define DEFINE_SNMP_STAT(type, name)	\
	__typeof__(type) __percpu *name[SNMP_ARRAY_SZ]
#define DEFINE_SNMP_STAT_ATOMIC(type, name)	\
	__typeof__(type) *name

#endif /* ! __NET_SNMP_H__ */
