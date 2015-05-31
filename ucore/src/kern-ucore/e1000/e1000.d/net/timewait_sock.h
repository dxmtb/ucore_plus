#ifndef __NET_TIMEWAIT_SOCK_H__
#define __NET_TIMEWAIT_SOCK_H__

#include <linux/slab.h>
#include <linux/bug.h>
#include <net/sock.h>

struct timewait_sock_ops {
	struct kmem_cache	*twsk_slab;
	char		*twsk_slab_name;
	unsigned int	twsk_obj_size;
	int		(*twsk_unique)(struct sock *sk,
				       struct sock *sktw, void *twp);
	void		(*twsk_destructor)(struct sock *sk);
};

#endif /* ! __NET_TIMEWAIT_SOCK_H__ */
