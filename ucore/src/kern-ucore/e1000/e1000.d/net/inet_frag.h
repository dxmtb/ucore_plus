#ifndef __NET_INET_FRAG_H__
#define __NET_INET_FRAG_H__

#include <linux/percpu_counter.h>

struct netns_frags {
	int			nqueues;
	struct list_head	lru_list;
	spinlock_t		lru_lock;

	/* The percpu_counter "mem" need to be cacheline aligned.
	 *  mem.count must not share cacheline with other writers
	 */
	struct percpu_counter   mem ____cacheline_aligned_in_smp;

	/* sysctls */
	int			timeout;
	int			high_thresh;
	int			low_thresh;
};

struct inet_frag_queue {
	spinlock_t		lock;
	struct timer_list	timer;      /* when will this queue expire? */
	struct list_head	lru_list;   /* lru list member */
	struct hlist_node	list;
	atomic_t		refcnt;
	struct sk_buff		*fragments; /* list of received fragments */
	struct sk_buff		*fragments_tail;
	ktime_t			stamp;
	int			len;        /* total length of orig datagram */
	int			meat;
	__u8			last_in;    /* first/last segment arrived? */

#define INET_FRAG_COMPLETE	4
#define INET_FRAG_FIRST_IN	2
#define INET_FRAG_LAST_IN	1

	u16			max_size;

	struct netns_frags	*net;
};

#define INETFRAGS_HASHSZ	1024

struct inet_frag_bucket {
	struct hlist_head	chain;
	spinlock_t		chain_lock;
};

struct inet_frags {
	struct inet_frag_bucket	hash[INETFRAGS_HASHSZ];
	/* This rwlock is a global lock (seperate per IPv4, IPv6 and
	 * netfilter). Important to keep this on a seperate cacheline.
	 * Its primarily a rebuild protection rwlock.
	 */
	rwlock_t		lock ____cacheline_aligned_in_smp;
	int			secret_interval;
	struct timer_list	secret_timer;
	u32			rnd;
	int			qsize;

	unsigned int		(*hashfn)(struct inet_frag_queue *);
	bool			(*match)(struct inet_frag_queue *q, void *arg);
	void			(*constructor)(struct inet_frag_queue *q,
						void *arg);
	void			(*destructor)(struct inet_frag_queue *);
	void			(*skb_free)(struct sk_buff *);
	void			(*frag_expire)(unsigned long data);
};

void inet_frag_destroy(struct inet_frag_queue *q,
				struct inet_frags *f, int *work);

/* The default percpu_counter batch size is not big enough to scale to
 * fragmentation mem acct sizes.
 * The mem size of a 64K fragment is approx:
 *  (44 fragments * 2944 truesize) + frag_queue struct(200) = 129736 bytes
 */
static unsigned int frag_percpu_counter_batch = 130000;

#endif /* ! __NET_INET_FRAG_H__ */
