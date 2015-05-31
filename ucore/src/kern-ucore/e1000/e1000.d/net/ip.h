#ifndef __NET_IP_H__
#define __NET_IP_H__

#include <linux/types.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/skbuff.h>

#include <net/inet_sock.h>
#include <net/snmp.h>
#include <net/flow.h>

struct sock;

#define IP_DF		0x4000		/* Flag: "Don't Fragment"	*/
#define IP_MF		0x2000		/* Flag: "More Fragments"	*/
#define IP_OFFSET	0x1FFF		/* "Fragment Offset" part	*/

struct msghdr;
struct net_device;

struct rtable;
struct sockaddr;

extern struct sk_buff  *__ip_make_skb(struct sock *sk,
				      struct flowi4 *fl4,
				      struct sk_buff_head *queue,
				      struct inet_cork *cork);

#define IP_REPLY_ARG_NOSRCCHECK 1

extern unsigned long snmp_fold_field(void __percpu *mib[], int offt);

extern unsigned long *sysctl_local_reserved_ports;

#include <net/dst.h>

extern void __ip_select_ident(struct iphdr *iph, struct dst_entry *dst, int more);

#include <linux/ipv6.h>

#endif /* ! __NET_IP_H__ */
