#ifndef __LINUX_NETLINK_H__
#define __LINUX_NETLINK_H__

#include <linux/capability.h>
#include <linux/skbuff.h>
#include <linux/export.h>
#include <net/scm.h>
#include <uapi/linux/netlink.h>

struct net;

#define NETLINK_CB(skb)		(*(struct netlink_skb_parms*)&((skb)->cb))

struct netlink_kernel_cfg {
	unsigned int	groups;
	unsigned int	flags;
	void		(*input)(struct sk_buff *skb);
	struct mutex	*cb_mutex;
	void		(*bind)(int group);
};

extern struct sock *__netlink_kernel_create(struct net *net, int unit,
					    struct module *module,
					    struct netlink_kernel_cfg *cfg);

extern int netlink_unicast(struct sock *ssk, struct sk_buff *skb, __u32 portid, int nonblock);
extern int netlink_broadcast(struct sock *ssk, struct sk_buff *skb, __u32 portid,
			     __u32 group, gfp_t allocation);

struct netlink_callback {
	struct sk_buff		*skb;
	const struct nlmsghdr	*nlh;
	int			(*dump)(struct sk_buff * skb,
					struct netlink_callback *cb);
	int			(*done)(struct netlink_callback *cb);
	void			*data;
	/* the module that dump function belong to */
	struct module		*module;
	u16			family;
	u16			min_dump_alloc;
	unsigned int		prev_seq, seq;
	long			args[6];
};

struct nlmsghdr *
__nlmsg_put(struct sk_buff *skb, u32 portid, u32 seq, int type, int len, int flags);

struct netlink_dump_control {
	int (*dump)(struct sk_buff *skb, struct netlink_callback *);
	int (*done)(struct netlink_callback *);
	void *data;
	struct module *module;
	u16 min_dump_alloc;
};

extern int __netlink_dump_start(struct sock *ssk, struct sk_buff *skb,
				const struct nlmsghdr *nlh,
				struct netlink_dump_control *control);

#endif /* ! __LINUX_NETLINK_H__ */
