#ifndef __ASM_MPSPEC_DEF_H__
#define __ASM_MPSPEC_DEF_H__

struct mpc_table {
	char signature[4];
	unsigned short length;		/* Size of table */
	char spec;			/* 0x01 */
	char checksum;
	char oem[8];
	char productid[12];
	unsigned int oemptr;		/* 0 if not present */
	unsigned short oemsize;		/* 0 if not present */
	unsigned short oemcount;
	unsigned int lapic;		/* APIC address */
	unsigned int reserved;
};

struct mpc_cpu {
	unsigned char type;
	unsigned char apicid;		/* Local APIC number */
	unsigned char apicver;		/* Its versions */
	unsigned char cpuflag;
	unsigned int cpufeature;
	unsigned int featureflag;	/* CPUID feature value */
	unsigned int reserved[2];
};

struct mpc_bus {
	unsigned char type;
	unsigned char busid;
	unsigned char bustype[6];
};

#endif /* ! __ASM_MPSPEC_DEF_H__ */
