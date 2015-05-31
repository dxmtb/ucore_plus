#ifndef __NET_NETNS_X_TABLES_H__
#define __NET_NETNS_X_TABLES_H__

#include <linux/list.h>
#include <linux/netfilter.h>

struct netns_xt {
	struct list_head tables[NFPROTO_NUMPROTO];
	bool notrack_deprecated_warning;
#if defined(CONFIG_BRIDGE_NF_EBTABLES) || \
    defined(CONFIG_BRIDGE_NF_EBTABLES_MODULE)
	struct ebt_table *broute_table;
	struct ebt_table *frame_filter;
	struct ebt_table *frame_nat;
#endif
};

#endif /* ! __NET_NETNS_X_TABLES_H__ */
