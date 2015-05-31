#ifndef __LINUX_MOD_DEVICETABLE_H__
#define __LINUX_MOD_DEVICETABLE_H__

#include <linux/types.h>
#include <linux/uuid.h>
typedef unsigned long kernel_ulong_t;

#define PCI_ANY_ID (~0)

struct pci_device_id {
	__u32 vendor, device;		/* Vendor and device ID or PCI_ANY_ID*/
	__u32 subvendor, subdevice;	/* Subsystem ID's or PCI_ANY_ID */
	__u32 class, class_mask;	/* (class,subclass,prog-if) triplet */
	kernel_ulong_t driver_data;	/* Data private to the driver */
};

#define ACPI_ID_LEN	9

struct acpi_device_id {
	__u8 id[ACPI_ID_LEN];
	kernel_ulong_t driver_data;
};

#define PNP_ID_LEN	8
#define PNP_MAX_DEVICES	8

/*
 * Struct used for matching a device
 */
struct of_device_id
{
	char	name[32];
	char	type[32];
	char	compatible[128];
	const void *data;
};

#define INPUT_DEVICE_ID_EV_MAX		0x1f

#define INPUT_DEVICE_ID_KEY_MAX		0x2ff
#define INPUT_DEVICE_ID_REL_MAX		0x0f
#define INPUT_DEVICE_ID_ABS_MAX		0x3f
#define INPUT_DEVICE_ID_MSC_MAX		0x07
#define INPUT_DEVICE_ID_LED_MAX		0x0f
#define INPUT_DEVICE_ID_SND_MAX		0x07
#define INPUT_DEVICE_ID_FF_MAX		0x7f
#define INPUT_DEVICE_ID_SW_MAX		0x0f

#define EISA_SIG_LEN   8

#define RPMSG_NAME_SIZE			32

#define I2C_NAME_SIZE	20

#define SPI_NAME_SIZE	32

#define PLATFORM_NAME_SIZE	20

#define MEI_CL_NAME_SIZE 32

#endif /* ! __LINUX_MOD_DEVICETABLE_H__ */
