#ifndef __UAPI_LINUX_MII_H__
#define __UAPI_LINUX_MII_H__

#include <linux/types.h>
#include <linux/ethtool.h>

#define ADVERTISE_10HALF	0x0020	/* Try for 10mbps half-duplex  */
#define ADVERTISE_1000XFULL	0x0020	/* Try for 1000BASE-X full-duplex */
#define ADVERTISE_10FULL	0x0040	/* Try for 10mbps full-duplex  */
#define ADVERTISE_1000XHALF	0x0040	/* Try for 1000BASE-X half-duplex */
#define ADVERTISE_100HALF	0x0080	/* Try for 100mbps half-duplex */
#define ADVERTISE_1000XPAUSE	0x0080	/* Try for 1000BASE-X pause    */
#define ADVERTISE_100FULL	0x0100	/* Try for 100mbps full-duplex */
#define ADVERTISE_1000XPSE_ASYM	0x0100	/* Try for 1000BASE-X asym pause */

#define ADVERTISE_PAUSE_CAP	0x0400	/* Try for pause               */
#define ADVERTISE_PAUSE_ASYM	0x0800	/* Try for asymetric pause     */

#define LPA_10HALF		0x0020	/* Can do 10mbps half-duplex   */

#define LPA_10FULL		0x0040	/* Can do 10mbps full-duplex   */

#define LPA_100HALF		0x0080	/* Can do 100mbps half-duplex  */

#define LPA_100FULL		0x0100	/* Can do 100mbps full-duplex  */

#define LPA_100BASE4		0x0200	/* Can do 100mbps 4k packets   */

#define LPA_LPACK		0x4000	/* Link partner acked us       */

#define LPA_DUPLEX		(LPA_10FULL | LPA_100FULL)

#define ADVERTISE_1000FULL	0x0200  /* Advertise 1000BASE-T full duplex */
#define ADVERTISE_1000HALF	0x0100  /* Advertise 1000BASE-T half duplex */

#define LPA_1000FULL		0x0800	/* Link partner 1000BASE-T full duplex */
#define LPA_1000HALF		0x0400	/* Link partner 1000BASE-T half duplex */

#define FLOW_CTRL_TX		0x01
#define FLOW_CTRL_RX		0x02

struct mii_ioctl_data {
	__u16		phy_id;
	__u16		reg_num;
	__u16		val_in;
	__u16		val_out;
};

#endif /* ! __UAPI_LINUX_MII_H__ */
