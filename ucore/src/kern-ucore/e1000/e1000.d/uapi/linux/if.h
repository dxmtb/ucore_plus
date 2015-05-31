#ifndef __UAPI_LINUX_IF_H__
#define __UAPI_LINUX_IF_H__

#include <linux/types.h>		/* for "__kernel_caddr_t" et al	*/
#include <linux/socket.h>		/* for "struct sockaddr" et al	*/
#include <linux/compiler.h>		/* for "__user" et al           */

#define	IFNAMSIZ	16

#include <linux/hdlc/ioctl.h>

#define	IFF_PROMISC	0x100		/* receive all packets		*/
#define	IFF_ALLMULTI	0x200		/* receive all multicast packets*/

#define IFF_MASTER	0x400		/* master of a load balancer 	*/
#define IFF_SLAVE	0x800		/* slave of a load balancer	*/

#define IFF_802_1Q_VLAN 0x1             /* 802.1Q VLAN device.          */

#define IFF_BONDING	0x20		/* bonding master or slave	*/

#define IFF_UNICAST_FLT	0x20000		/* Supports unicast filtering	*/

#define IFF_SUPP_NOFCS	0x80000		/* device supports sending custom FCS */

struct ifmap {
	unsigned long mem_start;
	unsigned long mem_end;
	unsigned short base_addr;
	unsigned char irq;
	unsigned char dma;
	unsigned char port;
	/* 3 bytes spare */
};

struct if_settings {
	unsigned int type;	/* Type of physical device or protocol */
	unsigned int size;	/* Size of the data allocated by the caller */
	union {
		/* {atm/eth/dsl}_settings anyone ? */
		raw_hdlc_proto		__user *raw_hdlc;
		cisco_proto		__user *cisco;
		fr_proto		__user *fr;
		fr_proto_pvc		__user *fr_pvc;
		fr_proto_pvc_info	__user *fr_pvc_info;

		/* interface settings */
		sync_serial_settings	__user *sync;
		te1_settings		__user *te1;
	} ifs_ifsu;
};

struct ifreq {
#define IFHWADDRLEN	6
	union
	{
		char	ifrn_name[IFNAMSIZ];		/* if name, e.g. "en0" */
	} ifr_ifrn;

	union {
		struct	sockaddr ifru_addr;
		struct	sockaddr ifru_dstaddr;
		struct	sockaddr ifru_broadaddr;
		struct	sockaddr ifru_netmask;
		struct  sockaddr ifru_hwaddr;
		short	ifru_flags;
		int	ifru_ivalue;
		int	ifru_mtu;
		struct  ifmap ifru_map;
		char	ifru_slave[IFNAMSIZ];	/* Just fits the size */
		char	ifru_newname[IFNAMSIZ];
		void __user *	ifru_data;
		struct	if_settings ifru_settings;
	} ifr_ifru;
};

#endif /* ! __UAPI_LINUX_IF_H__ */
