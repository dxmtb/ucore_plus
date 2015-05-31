#ifndef __ASM_CHECKSUM_64_H__
#define __ASM_CHECKSUM_64_H__

#include <linux/compiler.h>
#include <asm/uaccess.h>
#include <asm/byteorder.h>

/**
 * csum_fold - Fold and invert a 32bit checksum.
 * sum: 32bit unfolded sum
 *
 * Fold a 32bit running checksum to 16bit and invert it. This is usually
 * the last step before putting a checksum into a packet.
 * Make sure not to mix with 64bit checksums.
 */
static inline __sum16 csum_fold(__wsum sum)
{
	asm("  addl %1,%0\n"
	    "  adcl $0xffff,%0"
	    : "=r" (sum)
	    : "r" ((__force u32)sum << 16),
	      "0" ((__force u32)sum & 0xffff0000));
	return (__force __sum16)(~(__force u32)sum >> 16);
}

/**
 * csum_tcpup_nofold - Compute an IPv4 pseudo header checksum.
 * @saddr: source address
 * @daddr: destination address
 * @len: length of packet
 * @proto: ip protocol of packet
 * @sum: initial sum to be added in (32bit unfolded)
 *
 * Returns the pseudo header checksum the input data. Result is
 * 32bit unfolded.
 */
static inline __wsum
csum_tcpudp_nofold(__be32 saddr, __be32 daddr, unsigned short len,
		   unsigned short proto, __wsum sum)
{
	asm("  addl %1, %0\n"
	    "  adcl %2, %0\n"
	    "  adcl %3, %0\n"
	    "  adcl $0, %0\n"
	    : "=r" (sum)
	    : "g" (daddr), "g" (saddr),
	      "g" ((len + proto)<<8), "0" (sum));
	return sum;
}

/**
 * csum_tcpup_magic - Compute an IPv4 pseudo header checksum.
 * @saddr: source address
 * @daddr: destination address
 * @len: length of packet
 * @proto: ip protocol of packet
 * @sum: initial sum to be added in (32bit unfolded)
 *
 * Returns the 16bit pseudo header checksum the input data already
 * complemented and ready to be filled in.
 */
static inline __sum16 csum_tcpudp_magic(__be32 saddr, __be32 daddr,
					unsigned short len,
					unsigned short proto, __wsum sum)
{
	return csum_fold(csum_tcpudp_nofold(saddr, daddr, len, proto, sum));
}

/**
 * csum_partial - Compute an internet checksum.
 * @buff: buffer to be checksummed
 * @len: length of buffer.
 * @sum: initial sum to be added in (32bit unfolded)
 *
 * Returns the 32bit unfolded internet checksum of the buffer.
 * Before filling it in it needs to be csum_fold()'ed.
 * buff should be aligned to a 64bit boundary if possible.
 */
extern __wsum csum_partial(const void *buff, int len, __wsum sum);

#define  _HAVE_ARCH_COPY_AND_CSUM_FROM_USER 1
#define HAVE_CSUM_COPY_USER 1

extern __wsum csum_partial_copy_from_user(const void __user *src, void *dst,
					  int len, __wsum isum, int *errp);

#define csum_and_copy_from_user csum_partial_copy_from_user

struct in6_addr;

#define _HAVE_ARCH_IPV6_CSUM 1
extern __sum16
csum_ipv6_magic(const struct in6_addr *saddr, const struct in6_addr *daddr,
		__u32 len, unsigned short proto, __wsum sum);

#endif /* ! __ASM_CHECKSUM_64_H__ */
