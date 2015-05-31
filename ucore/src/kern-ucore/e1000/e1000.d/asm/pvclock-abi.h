#ifndef __ASM_PVCLOCK_ABI_H__
#define __ASM_PVCLOCK_ABI_H__

struct pvclock_vcpu_time_info {
	u32   version;
	u32   pad0;
	u64   tsc_timestamp;
	u64   system_time;
	u32   tsc_to_system_mul;
	s8    tsc_shift;
	u8    flags;
	u8    pad[2];
} __attribute__((__packed__)); /* 32 bytes */

#endif /* ! __ASM_PVCLOCK_ABI_H__ */
