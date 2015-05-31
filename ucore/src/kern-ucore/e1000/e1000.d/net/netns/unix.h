#ifndef __NET_NETNS_UNIX_H__
#define __NET_NETNS_UNIX_H__

struct ctl_table_header;
struct netns_unix {
	int			sysctl_max_dgram_qlen;
	struct ctl_table_header	*ctl;
};

#endif /* ! __NET_NETNS_UNIX_H__ */
