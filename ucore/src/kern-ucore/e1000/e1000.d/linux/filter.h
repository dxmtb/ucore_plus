#ifndef __LINUX_FILTER_H__
#define __LINUX_FILTER_H__

#include <linux/atomic.h>
#include <linux/compat.h>
#include <uapi/linux/filter.h>

struct sk_buff;
struct sock;

struct sk_filter
{
	atomic_t		refcnt;
	unsigned int         	len;	/* Number of filter blocks */
	unsigned int		(*bpf_func)(const struct sk_buff *skb,
					    const struct sock_filter *filter);
	struct rcu_head		rcu;
	struct sock_filter     	insns[0];
};

static inline unsigned int sk_filter_len(const struct sk_filter *fp)
{
	return fp->len * sizeof(struct sock_filter) + sizeof(*fp);
}

#endif /* ! __LINUX_FILTER_H__ */
