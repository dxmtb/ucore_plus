#ifndef __UAPI_LINUX_ETHTOOL_H__
#define __UAPI_LINUX_ETHTOOL_H__

#include <linux/types.h>
#include <linux/if_ether.h>

struct ethtool_cmd {
	__u32	cmd;
	__u32	supported;	/* Features this interface supports */
	__u32	advertising;	/* Features this interface advertises */
	__u16	speed;	        /* The forced speed (lower bits) in
				 * Mbps. Please use
				 * ethtool_cmd_speed()/_set() to
				 * access it */
	__u8	duplex;		/* Duplex, half or full */
	__u8	port;		/* Which connector port */
	__u8	phy_address;	/* MDIO PHY address (PRTAD for clause 45).
				 * May be read-only or read-write
				 * depending on the driver.
				 */
	__u8	transceiver;	/* Which transceiver to use */
	__u8	autoneg;	/* Enable or disable autonegotiation */
	__u8	mdio_support;	/* MDIO protocols supported.  Read-only.
				 * Not set by all drivers.
				 */
	__u32	maxtxpkt;	/* Tx pkts before generating tx int */
	__u32	maxrxpkt;	/* Rx pkts before generating rx int */
	__u16	speed_hi;       /* The forced speed (upper
				 * bits) in Mbps. Please use
				 * ethtool_cmd_speed()/_set() to
				 * access it */
	__u8	eth_tp_mdix;	/* twisted pair MDI-X status */
	__u8    eth_tp_mdix_ctrl; /* twisted pair MDI-X control, when set,
				   * link should be renegotiated if necessary
				   */
	__u32	lp_advertising;	/* Features the link partner advertises */
	__u32	reserved[2];
};

static inline void ethtool_cmd_speed_set(struct ethtool_cmd *ep,
					 __u32 speed)
{

	ep->speed = (__u16)speed;
	ep->speed_hi = (__u16)(speed >> 16);
}

static inline __u32 ethtool_cmd_speed(const struct ethtool_cmd *ep)
{
	return (ep->speed_hi << 16) | ep->speed;
}

#define ETHTOOL_FWVERS_LEN	32
#define ETHTOOL_BUSINFO_LEN	32

struct ethtool_drvinfo {
	__u32	cmd;
	char	driver[32];	/* driver short name, "tulip", "eepro100" */
	char	version[32];	/* driver version string */
	char	fw_version[ETHTOOL_FWVERS_LEN];	/* firmware version string */
	char	bus_info[ETHTOOL_BUSINFO_LEN];	/* Bus info for this IF. */
				/* For PCI devices, use pci_name(pci_dev). */
	char	reserved1[32];
	char	reserved2[12];
				/*
				 * Some struct members below are filled in
				 * using ops->get_sset_count().  Obtaining
				 * this info from ethtool_drvinfo is now
				 * deprecated; Use ETHTOOL_GSSET_INFO
				 * instead.
				 */
	__u32	n_priv_flags;	/* number of flags valid in ETHTOOL_GPFLAGS */
	__u32	n_stats;	/* number of u64's from ETHTOOL_GSTATS */
	__u32	testinfo_len;
	__u32	eedump_len;	/* Size of data from ETHTOOL_GEEPROM (bytes) */
	__u32	regdump_len;	/* Size of data from ETHTOOL_GREGS (bytes) */
};

#define SOPASS_MAX	6

struct ethtool_wolinfo {
	__u32	cmd;
	__u32	supported;
	__u32	wolopts;
	__u8	sopass[SOPASS_MAX]; /* SecureOn(tm) password */
};

struct ethtool_regs {
	__u32	cmd;
	__u32	version; /* driver-specific, indicates different chips/revs */
	__u32	len; /* bytes */
	__u8	data[0];
};

struct ethtool_eeprom {
	__u32	cmd;
	__u32	magic;
	__u32	offset; /* in bytes */
	__u32	len; /* in bytes */
	__u8	data[0];
};

/**
 * struct ethtool_eee - Energy Efficient Ethernet information
 * @cmd: ETHTOOL_{G,S}EEE
 * @supported: Mask of %SUPPORTED_* flags for the speed/duplex combinations
 *	for which there is EEE support.
 * @advertised: Mask of %ADVERTISED_* flags for the speed/duplex combinations
 *	advertised as eee capable.
 * @lp_advertised: Mask of %ADVERTISED_* flags for the speed/duplex
 *	combinations advertised by the link partner as eee capable.
 * @eee_active: Result of the eee auto negotiation.
 * @eee_enabled: EEE configured mode (enabled/disabled).
 * @tx_lpi_enabled: Whether the interface should assert its tx lpi, given
 *	that eee was negotiated.
 * @tx_lpi_timer: Time in microseconds the interface delays prior to asserting
 *	its tx lpi (after reaching 'idle' state). Effective only when eee
 *	was negotiated and tx_lpi_enabled was set.
 */
struct ethtool_eee {
	__u32	cmd;
	__u32	supported;
	__u32	advertised;
	__u32	lp_advertised;
	__u32	eee_active;
	__u32	eee_enabled;
	__u32	tx_lpi_enabled;
	__u32	tx_lpi_timer;
	__u32	reserved[2];
};

/**
 * struct ethtool_modinfo - plugin module eeprom information
 * @cmd: %ETHTOOL_GMODULEINFO
 * @type: Standard the module information conforms to %ETH_MODULE_SFF_xxxx
 * @eeprom_len: Length of the eeprom
 *
 * This structure is used to return the information to
 * properly size memory for a subsequent call to %ETHTOOL_GMODULEEEPROM.
 * The type code indicates the eeprom data format
 */
struct ethtool_modinfo {
	__u32   cmd;
	__u32   type;
	__u32   eeprom_len;
	__u32   reserved[8];
};

/**
 * struct ethtool_coalesce - coalescing parameters for IRQs and stats updates
 * @cmd: ETHTOOL_{G,S}COALESCE
 * @rx_coalesce_usecs: How many usecs to delay an RX interrupt after
 *	a packet arrives.
 * @rx_max_coalesced_frames: Maximum number of packets to receive
 *	before an RX interrupt.
 * @rx_coalesce_usecs_irq: Same as @rx_coalesce_usecs, except that
 *	this value applies while an IRQ is being serviced by the host.
 * @rx_max_coalesced_frames_irq: Same as @rx_max_coalesced_frames,
 *	except that this value applies while an IRQ is being serviced
 *	by the host.
 * @tx_coalesce_usecs: How many usecs to delay a TX interrupt after
 *	a packet is sent.
 * @tx_max_coalesced_frames: Maximum number of packets to be sent
 *	before a TX interrupt.
 * @tx_coalesce_usecs_irq: Same as @tx_coalesce_usecs, except that
 *	this value applies while an IRQ is being serviced by the host.
 * @tx_max_coalesced_frames_irq: Same as @tx_max_coalesced_frames,
 *	except that this value applies while an IRQ is being serviced
 *	by the host.
 * @stats_block_coalesce_usecs: How many usecs to delay in-memory
 *	statistics block updates.  Some drivers do not have an
 *	in-memory statistic block, and in such cases this value is
 *	ignored.  This value must not be zero.
 * @use_adaptive_rx_coalesce: Enable adaptive RX coalescing.
 * @use_adaptive_tx_coalesce: Enable adaptive TX coalescing.
 * @pkt_rate_low: Threshold for low packet rate (packets per second).
 * @rx_coalesce_usecs_low: How many usecs to delay an RX interrupt after
 *	a packet arrives, when the packet rate is below @pkt_rate_low.
 * @rx_max_coalesced_frames_low: Maximum number of packets to be received
 *	before an RX interrupt, when the packet rate is below @pkt_rate_low.
 * @tx_coalesce_usecs_low: How many usecs to delay a TX interrupt after
 *	a packet is sent, when the packet rate is below @pkt_rate_low.
 * @tx_max_coalesced_frames_low: Maximum nuumber of packets to be sent before
 *	a TX interrupt, when the packet rate is below @pkt_rate_low.
 * @pkt_rate_high: Threshold for high packet rate (packets per second).
 * @rx_coalesce_usecs_high: How many usecs to delay an RX interrupt after
 *	a packet arrives, when the packet rate is above @pkt_rate_high.
 * @rx_max_coalesced_frames_high: Maximum number of packets to be received
 *	before an RX interrupt, when the packet rate is above @pkt_rate_high.
 * @tx_coalesce_usecs_high: How many usecs to delay a TX interrupt after
 *	a packet is sent, when the packet rate is above @pkt_rate_high.
 * @tx_max_coalesced_frames_high: Maximum number of packets to be sent before
 *	a TX interrupt, when the packet rate is above @pkt_rate_high.
 * @rate_sample_interval: How often to do adaptive coalescing packet rate
 *	sampling, measured in seconds.  Must not be zero.
 *
 * Each pair of (usecs, max_frames) fields specifies this exit
 * condition for interrupt coalescing:
 *	(usecs > 0 && time_since_first_completion >= usecs) ||
 *	(max_frames > 0 && completed_frames >= max_frames)
 * It is illegal to set both usecs and max_frames to zero as this
 * would cause interrupts to never be generated.  To disable
 * coalescing, set usecs = 0 and max_frames = 1.
 *
 * Some implementations ignore the value of max_frames and use the
 * condition:
 *	time_since_first_completion >= usecs
 * This is deprecated.  Drivers for hardware that does not support
 * counting completions should validate that max_frames == !rx_usecs.
 *
 * Adaptive RX/TX coalescing is an algorithm implemented by some
 * drivers to improve latency under low packet rates and improve
 * throughput under high packet rates.  Some drivers only implement
 * one of RX or TX adaptive coalescing.  Anything not implemented by
 * the driver causes these values to be silently ignored.
 *
 * When the packet rate is below @pkt_rate_high but above
 * @pkt_rate_low (both measured in packets per second) the
 * normal {rx,tx}_* coalescing parameters are used.
 */
struct ethtool_coalesce {
	__u32	cmd;
	__u32	rx_coalesce_usecs;
	__u32	rx_max_coalesced_frames;
	__u32	rx_coalesce_usecs_irq;
	__u32	rx_max_coalesced_frames_irq;
	__u32	tx_coalesce_usecs;
	__u32	tx_max_coalesced_frames;
	__u32	tx_coalesce_usecs_irq;
	__u32	tx_max_coalesced_frames_irq;
	__u32	stats_block_coalesce_usecs;
	__u32	use_adaptive_rx_coalesce;
	__u32	use_adaptive_tx_coalesce;
	__u32	pkt_rate_low;
	__u32	rx_coalesce_usecs_low;
	__u32	rx_max_coalesced_frames_low;
	__u32	tx_coalesce_usecs_low;
	__u32	tx_max_coalesced_frames_low;
	__u32	pkt_rate_high;
	__u32	rx_coalesce_usecs_high;
	__u32	rx_max_coalesced_frames_high;
	__u32	tx_coalesce_usecs_high;
	__u32	tx_max_coalesced_frames_high;
	__u32	rate_sample_interval;
};

struct ethtool_ringparam {
	__u32	cmd;	/* ETHTOOL_{G,S}RINGPARAM */

	/* Read only attributes.  These indicate the maximum number
	 * of pending RX/TX ring entries the driver will allow the
	 * user to set.
	 */
	__u32	rx_max_pending;
	__u32	rx_mini_max_pending;
	__u32	rx_jumbo_max_pending;
	__u32	tx_max_pending;

	/* Values changeable by the user.  The valid values are
	 * in the range 1 to the "*_max_pending" counterpart above.
	 */
	__u32	rx_pending;
	__u32	rx_mini_pending;
	__u32	rx_jumbo_pending;
	__u32	tx_pending;
};

struct ethtool_channels {
	__u32	cmd;
	__u32	max_rx;
	__u32	max_tx;
	__u32	max_other;
	__u32	max_combined;
	__u32	rx_count;
	__u32	tx_count;
	__u32	other_count;
	__u32	combined_count;
};

struct ethtool_pauseparam {
	__u32	cmd;	/* ETHTOOL_{G,S}PAUSEPARAM */

	/* If the link is being auto-negotiated (via ethtool_cmd.autoneg
	 * being true) the user may set 'autoneg' here non-zero to have the
	 * pause parameters be auto-negotiated too.  In such a case, the
	 * {rx,tx}_pause values below determine what capabilities are
	 * advertised.
	 *
	 * If 'autoneg' is zero or the link is not being auto-negotiated,
	 * then {rx,tx}_pause force the driver to use/not-use pause
	 * flow control.
	 */
	__u32	autoneg;
	__u32	rx_pause;
	__u32	tx_pause;
};

#define ETH_GSTRING_LEN		32
enum ethtool_stringset {
	ETH_SS_TEST		= 0,
	ETH_SS_STATS,
	ETH_SS_PRIV_FLAGS,
	ETH_SS_NTUPLE_FILTERS,	/* Do not use, GRXNTUPLE is now deprecated */
	ETH_SS_FEATURES,
};

enum ethtool_test_flags {
	ETH_TEST_FL_OFFLINE	= (1 << 0),
	ETH_TEST_FL_FAILED	= (1 << 1),
	ETH_TEST_FL_EXTERNAL_LB	= (1 << 2),
	ETH_TEST_FL_EXTERNAL_LB_DONE	= (1 << 3),
};

struct ethtool_test {
	__u32	cmd;		/* ETHTOOL_TEST */
	__u32	flags;		/* ETH_TEST_FL_xxx */
	__u32	reserved;
	__u32	len;		/* result length, in number of u64 elements */
	__u64	data[0];
};

struct ethtool_stats {
	__u32	cmd;		/* ETHTOOL_GSTATS */
	__u32	n_stats;	/* number of u64's being returned */
	__u64	data[0];
};

/**
 * struct ethtool_tcpip4_spec - flow specification for TCP/IPv4 etc.
 * @ip4src: Source host
 * @ip4dst: Destination host
 * @psrc: Source port
 * @pdst: Destination port
 * @tos: Type-of-service
 *
 * This can be used to specify a TCP/IPv4, UDP/IPv4 or SCTP/IPv4 flow.
 */
struct ethtool_tcpip4_spec {
	__be32	ip4src;
	__be32	ip4dst;
	__be16	psrc;
	__be16	pdst;
	__u8    tos;
};

/**
 * struct ethtool_ah_espip4_spec - flow specification for IPsec/IPv4
 * @ip4src: Source host
 * @ip4dst: Destination host
 * @spi: Security parameters index
 * @tos: Type-of-service
 *
 * This can be used to specify an IPsec transport or tunnel over IPv4.
 */
struct ethtool_ah_espip4_spec {
	__be32	ip4src;
	__be32	ip4dst;
	__be32	spi;
	__u8    tos;
};

/**
 * struct ethtool_usrip4_spec - general flow specification for IPv4
 * @ip4src: Source host
 * @ip4dst: Destination host
 * @l4_4_bytes: First 4 bytes of transport (layer 4) header
 * @tos: Type-of-service
 * @ip_ver: Value must be %ETH_RX_NFC_IP4; mask must be 0
 * @proto: Transport protocol number; mask must be 0
 */
struct ethtool_usrip4_spec {
	__be32	ip4src;
	__be32	ip4dst;
	__be32	l4_4_bytes;
	__u8    tos;
	__u8    ip_ver;
	__u8    proto;
};

union ethtool_flow_union {
	struct ethtool_tcpip4_spec		tcp_ip4_spec;
	struct ethtool_tcpip4_spec		udp_ip4_spec;
	struct ethtool_tcpip4_spec		sctp_ip4_spec;
	struct ethtool_ah_espip4_spec		ah_ip4_spec;
	struct ethtool_ah_espip4_spec		esp_ip4_spec;
	struct ethtool_usrip4_spec		usr_ip4_spec;
	struct ethhdr				ether_spec;
	__u8					hdata[52];
};

/**
 * struct ethtool_flow_ext - additional RX flow fields
 * @h_dest: destination MAC address
 * @vlan_etype: VLAN EtherType
 * @vlan_tci: VLAN tag control information
 * @data: user defined data
 *
 * Note, @vlan_etype, @vlan_tci, and @data are only valid if %FLOW_EXT
 * is set in &struct ethtool_rx_flow_spec @flow_type.
 * @h_dest is valid if %FLOW_MAC_EXT is set.
 */
struct ethtool_flow_ext {
	__u8		padding[2];
	unsigned char	h_dest[ETH_ALEN];
	__be16		vlan_etype;
	__be16		vlan_tci;
	__be32		data[2];
};

/**
 * struct ethtool_rx_flow_spec - classification rule for RX flows
 * @flow_type: Type of match to perform, e.g. %TCP_V4_FLOW
 * @h_u: Flow fields to match (dependent on @flow_type)
 * @h_ext: Additional fields to match
 * @m_u: Masks for flow field bits to be matched
 * @m_ext: Masks for additional field bits to be matched
 *	Note, all additional fields must be ignored unless @flow_type
 *	includes the %FLOW_EXT or %FLOW_MAC_EXT flag
 *	(see &struct ethtool_flow_ext description).
 * @ring_cookie: RX ring/queue index to deliver to, or %RX_CLS_FLOW_DISC
 *	if packets should be discarded
 * @location: Location of rule in the table.  Locations must be
 *	numbered such that a flow matching multiple rules will be
 *	classified according to the first (lowest numbered) rule.
 */
struct ethtool_rx_flow_spec {
	__u32		flow_type;
	union ethtool_flow_union h_u;
	struct ethtool_flow_ext h_ext;
	union ethtool_flow_union m_u;
	struct ethtool_flow_ext m_ext;
	__u64		ring_cookie;
	__u32		location;
};

/**
 * struct ethtool_rxnfc - command to get or set RX flow classification rules
 * @cmd: Specific command number - %ETHTOOL_GRXFH, %ETHTOOL_SRXFH,
 *	%ETHTOOL_GRXRINGS, %ETHTOOL_GRXCLSRLCNT, %ETHTOOL_GRXCLSRULE,
 *	%ETHTOOL_GRXCLSRLALL, %ETHTOOL_SRXCLSRLDEL or %ETHTOOL_SRXCLSRLINS
 * @flow_type: Type of flow to be affected, e.g. %TCP_V4_FLOW
 * @data: Command-dependent value
 * @fs: Flow classification rule
 * @rule_cnt: Number of rules to be affected
 * @rule_locs: Array of used rule locations
 *
 * For %ETHTOOL_GRXFH and %ETHTOOL_SRXFH, @data is a bitmask indicating
 * the fields included in the flow hash, e.g. %RXH_IP_SRC.  The following
 * structure fields must not be used.
 *
 * For %ETHTOOL_GRXRINGS, @data is set to the number of RX rings/queues
 * on return.
 *
 * For %ETHTOOL_GRXCLSRLCNT, @rule_cnt is set to the number of defined
 * rules on return.  If @data is non-zero on return then it is the
 * size of the rule table, plus the flag %RX_CLS_LOC_SPECIAL if the
 * driver supports any special location values.  If that flag is not
 * set in @data then special location values should not be used.
 *
 * For %ETHTOOL_GRXCLSRULE, @fs.@location specifies the location of an
 * existing rule on entry and @fs contains the rule on return.
 *
 * For %ETHTOOL_GRXCLSRLALL, @rule_cnt specifies the array size of the
 * user buffer for @rule_locs on entry.  On return, @data is the size
 * of the rule table, @rule_cnt is the number of defined rules, and
 * @rule_locs contains the locations of the defined rules.  Drivers
 * must use the second parameter to get_rxnfc() instead of @rule_locs.
 *
 * For %ETHTOOL_SRXCLSRLINS, @fs specifies the rule to add or update.
 * @fs.@location either specifies the location to use or is a special
 * location value with %RX_CLS_LOC_SPECIAL flag set.  On return,
 * @fs.@location is the actual rule location.
 *
 * For %ETHTOOL_SRXCLSRLDEL, @fs.@location specifies the location of an
 * existing rule on entry.
 *
 * A driver supporting the special location values for
 * %ETHTOOL_SRXCLSRLINS may add the rule at any suitable unused
 * location, and may remove a rule at a later location (lower
 * priority) that matches exactly the same set of flows.  The special
 * values are: %RX_CLS_LOC_ANY, selecting any location;
 * %RX_CLS_LOC_FIRST, selecting the first suitable location (maximum
 * priority); and %RX_CLS_LOC_LAST, selecting the last suitable
 * location (minimum priority).  Additional special values may be
 * defined in future and drivers must return -%EINVAL for any
 * unrecognised value.
 */
struct ethtool_rxnfc {
	__u32				cmd;
	__u32				flow_type;
	__u64				data;
	struct ethtool_rx_flow_spec	fs;
	__u32				rule_cnt;
	__u32				rule_locs[0];
};

#define ETHTOOL_FLASH_MAX_FILENAME	128

struct ethtool_flash {
	__u32	cmd;
	__u32	region;
	char	data[ETHTOOL_FLASH_MAX_FILENAME];
};

struct ethtool_dump {
	__u32	cmd;
	__u32	version;
	__u32	flag;
	__u32	len;
	__u8	data[0];
};

/**
 * struct ethtool_ts_info - holds a device's timestamping and PHC association
 * @cmd: command number = %ETHTOOL_GET_TS_INFO
 * @so_timestamping: bit mask of the sum of the supported SO_TIMESTAMPING flags
 * @phc_index: device index of the associated PHC, or -1 if there is none
 * @tx_types: bit mask of the supported hwtstamp_tx_types enumeration values
 * @rx_filters: bit mask of the supported hwtstamp_rx_filters enumeration values
 *
 * The bits in the 'tx_types' and 'rx_filters' fields correspond to
 * the 'hwtstamp_tx_types' and 'hwtstamp_rx_filters' enumeration values,
 * respectively.  For example, if the device supports HWTSTAMP_TX_ON,
 * then (1 << HWTSTAMP_TX_ON) in 'tx_types' will be set.
 */
struct ethtool_ts_info {
	__u32	cmd;
	__u32	so_timestamping;
	__s32	phc_index;
	__u32	tx_types;
	__u32	tx_reserved[3];
	__u32	rx_filters;
	__u32	rx_reserved[3];
};

#define SUPPORTED_10baseT_Half		(1 << 0)
#define SUPPORTED_10baseT_Full		(1 << 1)
#define SUPPORTED_100baseT_Half		(1 << 2)
#define SUPPORTED_100baseT_Full		(1 << 3)

#define SUPPORTED_1000baseT_Full	(1 << 5)
#define SUPPORTED_Autoneg		(1 << 6)
#define SUPPORTED_TP			(1 << 7)

#define SUPPORTED_FIBRE			(1 << 10)

#define ADVERTISED_10baseT_Half		(1 << 0)
#define ADVERTISED_10baseT_Full		(1 << 1)
#define ADVERTISED_100baseT_Half	(1 << 2)
#define ADVERTISED_100baseT_Full	(1 << 3)
#define ADVERTISED_1000baseT_Half	(1 << 4)
#define ADVERTISED_1000baseT_Full	(1 << 5)
#define ADVERTISED_Autoneg		(1 << 6)
#define ADVERTISED_TP			(1 << 7)

#define ADVERTISED_FIBRE		(1 << 10)

#define ADVERTISED_Pause		(1 << 13)
#define ADVERTISED_Asym_Pause		(1 << 14)

#define DUPLEX_HALF		0x00
#define DUPLEX_FULL		0x01

#define PORT_TP			0x00

#define PORT_FIBRE		0x03

#define XCVR_INTERNAL		0x00
#define XCVR_EXTERNAL		0x01

/* Enable or disable autonegotiation.  If this is set to enable,
 * the forced link modes above are completely ignored.
 */
#define AUTONEG_DISABLE		0x00
#define AUTONEG_ENABLE		0x01

/* MDI or MDI-X status/control - if MDI/MDI_X/AUTO is set then
 * the driver is required to renegotiate link
 */
#define ETH_TP_MDI_INVALID	0x00 /* status: unknown; control: unsupported */
#define ETH_TP_MDI		0x01 /* status: MDI;     control: force MDI */
#define ETH_TP_MDI_X		0x02 /* status: MDI-X;   control: force MDI-X */
#define ETH_TP_MDI_AUTO		0x03 /*                  control: auto-select */

#define WAKE_PHY		(1 << 0)
#define WAKE_UCAST		(1 << 1)
#define WAKE_MCAST		(1 << 2)
#define WAKE_BCAST		(1 << 3)
#define WAKE_ARP		(1 << 4)
#define WAKE_MAGIC		(1 << 5)
#define WAKE_MAGICSECURE	(1 << 6) /* only meaningful if WAKE_MAGIC */

#endif /* ! __UAPI_LINUX_ETHTOOL_H__ */
