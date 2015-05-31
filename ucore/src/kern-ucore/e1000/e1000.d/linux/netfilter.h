#ifndef __LINUX_NETFILTER_H__
#define __LINUX_NETFILTER_H__

#include <linux/init.h>
#include <linux/skbuff.h>
#include <linux/net.h>
#include <linux/if.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/wait.h>
#include <linux/list.h>
#include <uapi/linux/netfilter.h>

#define NF_MAX_HOOKS 8

struct sk_buff;

extern struct list_head nf_hooks[NFPROTO_NUMPROTO][NF_MAX_HOOKS];

static inline bool nf_hooks_active(u_int8_t pf, unsigned int hook)
{
	return !list_empty(&nf_hooks[pf][hook]);
}

int nf_hook_slow(u_int8_t pf, unsigned int hook, struct sk_buff *skb,
		 struct net_device *indev, struct net_device *outdev,
		 int (*okfn)(struct sk_buff *), int thresh);

/**
 *	nf_hook_thresh - call a netfilter hook
 *
 *	Returns 1 if the hook has allowed the packet to pass.  The function
 *	okfn must be invoked by the caller in this case.  Any other return
 *	value indicates the packet has been consumed by the hook.
 */
static inline int nf_hook_thresh(u_int8_t pf, unsigned int hook,
				 struct sk_buff *skb,
				 struct net_device *indev,
				 struct net_device *outdev,
				 int (*okfn)(struct sk_buff *), int thresh)
{
	if (nf_hooks_active(pf, hook))
		return nf_hook_slow(pf, hook, skb, indev, outdev, okfn, thresh);
	return 1;
}

static inline int
NF_HOOK_THRESH(uint8_t pf, unsigned int hook, struct sk_buff *skb,
	       struct net_device *in, struct net_device *out,
	       int (*okfn)(struct sk_buff *), int thresh)
{
	int ret = nf_hook_thresh(pf, hook, skb, in, out, okfn, thresh);
	if (ret == 1)
		ret = okfn(skb);
	return ret;
}

struct flowi;
struct nf_queue_entry;

struct nf_afinfo {
	unsigned short	family;
	__sum16		(*checksum)(struct sk_buff *skb, unsigned int hook,
				    unsigned int dataoff, u_int8_t protocol);
	__sum16		(*checksum_partial)(struct sk_buff *skb,
					    unsigned int hook,
					    unsigned int dataoff,
					    unsigned int len,
					    u_int8_t protocol);
	int		(*route)(struct net *net, struct dst_entry **dst,
				 struct flowi *fl, bool strict);
	void		(*saveroute)(const struct sk_buff *skb,
				     struct nf_queue_entry *entry);
	int		(*reroute)(struct sk_buff *skb,
				   const struct nf_queue_entry *entry);
	int		route_key_size;
};

extern const struct nf_afinfo __rcu *nf_afinfo[NFPROTO_NUMPROTO];
static inline const struct nf_afinfo *nf_get_afinfo(unsigned short family)
{
	return rcu_dereference(nf_afinfo[family]);
}

#include <net/flow.h>
extern void (*nf_nat_decode_session_hook)(struct sk_buff *, struct flowi *);

struct nlattr;

#endif /* ! __LINUX_NETFILTER_H__ */
