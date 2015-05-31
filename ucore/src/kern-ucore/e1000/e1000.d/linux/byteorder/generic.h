#ifndef __LINUX_BYTEORDER_GENERIC_H__
#define __LINUX_BYTEORDER_GENERIC_H__

#define cpu_to_le64 __cpu_to_le64
#define le64_to_cpu __le64_to_cpu
#define cpu_to_le32 __cpu_to_le32
#define le32_to_cpu __le32_to_cpu
#define cpu_to_le16 __cpu_to_le16
#define le16_to_cpu __le16_to_cpu
#define cpu_to_be64 __cpu_to_be64
#define be64_to_cpu __be64_to_cpu
#define cpu_to_be32 __cpu_to_be32
#define be32_to_cpu __be32_to_cpu
#define cpu_to_be16 __cpu_to_be16
#define be16_to_cpu __be16_to_cpu

#define le64_to_cpup __le64_to_cpup

#define le32_to_cpup __le32_to_cpup

#define le16_to_cpup __le16_to_cpup

#define be64_to_cpup __be64_to_cpup

#define be32_to_cpup __be32_to_cpup

#define be16_to_cpup __be16_to_cpup

#define le16_to_cpus __le16_to_cpus

#define ___htonl(x) __cpu_to_be32(x)
#define ___htons(x) __cpu_to_be16(x)
#define ___ntohl(x) __be32_to_cpu(x)
#define ___ntohs(x) __be16_to_cpu(x)

#define htonl(x) ___htonl(x)
#define ntohl(x) ___ntohl(x)
#define htons(x) ___htons(x)
#define ntohs(x) ___ntohs(x)

#endif /* ! __LINUX_BYTEORDER_GENERIC_H__ */
