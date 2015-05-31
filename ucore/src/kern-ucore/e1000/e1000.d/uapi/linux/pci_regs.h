#ifndef __UAPI_LINUX_PCI_REGS_H__
#define __UAPI_LINUX_PCI_REGS_H__

#define PCI_COMMAND		0x04	/* 16 bits */

#define  PCI_COMMAND_INVALIDATE	0x10	/* Use memory write and invalidate */

#define PCI_EXP_FLAGS_TYPE	0x00f0	/* Device/Port type */

#endif /* ! __UAPI_LINUX_PCI_REGS_H__ */
