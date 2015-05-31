#ifndef __UAPI_LINUX_IF_ETHER_H__
#define __UAPI_LINUX_IF_ETHER_H__

#include <linux/types.h>

#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#define ETH_HLEN	14		/* Total octets in header.	 */
#define ETH_ZLEN	60		/* Min. octets in frame sans FCS */
#define ETH_DATA_LEN	1500		/* Max. octets in payload	 */
#define ETH_FRAME_LEN	1514		/* Max. octets in frame sans FCS */
#define ETH_FCS_LEN	4		/* Octets in the FCS		 */

#define ETH_P_IP	0x0800		/* Internet Protocol packet	*/

#define ETH_P_8021Q	0x8100          /* 802.1Q VLAN Extended Header  */

#define ETH_P_IPV6	0x86DD		/* IPv6 over bluebook		*/

#define ETH_P_8021AD	0x88A8          /* 802.1ad Service VLAN		*/

#define ETH_P_FCOE	0x8906		/* Fibre Channel over Ethernet  */

#define ETH_P_802_3_MIN	0x0600		/* If the value in the ethernet type is less than this value
					 * then the frame is Ethernet II. Else it is 802.3 */

#define ETH_P_802_3	0x0001		/* Dummy type for 802.3 frames  */

#define ETH_P_802_2	0x0004		/* 802.2 frames 		*/

#define ETH_P_DSA	0x001B		/* Distributed Switch Arch.	*/
#define ETH_P_TRAILER	0x001C		/* Trailer switch tagging	*/

struct ethhdr {
	unsigned char	h_dest[ETH_ALEN];	/* destination eth addr	*/
	unsigned char	h_source[ETH_ALEN];	/* source ether addr	*/
	__be16		h_proto;		/* packet type ID field	*/
} __attribute__((packed));

#endif /* ! __UAPI_LINUX_IF_ETHER_H__ */
