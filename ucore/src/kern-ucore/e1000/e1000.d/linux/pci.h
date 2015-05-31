#ifndef __LINUX_PCI_H__
#define __LINUX_PCI_H__

#include <linux/mod_devicetable.h>

#include <linux/types.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/list.h>
#include <linux/compiler.h>
#include <linux/errno.h>
#include <linux/kobject.h>
#include <linux/atomic.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/irqreturn.h>
#include <uapi/linux/pci.h>

#include <linux/pci_ids.h>

struct pci_slot {
	struct pci_bus *bus;		/* The bus this slot is on */
	struct list_head list;		/* node in list of slots on this bus */
	struct hotplug_slot *hotplug;	/* Hotplug info (migrate over time) */
	unsigned char number;		/* PCI_SLOT(pci_dev->devfn) */
	struct kobject kobj;
};

/*
 *  For PCI devices, the region numbers are assigned this way:
 */
enum {
	/* #0-5: standard PCI resources */
	PCI_STD_RESOURCES,
	PCI_STD_RESOURCE_END = 5,

	/* #6: expansion ROM resource */
	PCI_ROM_RESOURCE,

	/* device specific resources */
#ifdef CONFIG_PCI_IOV
	PCI_IOV_RESOURCES,
	PCI_IOV_RESOURCE_END = PCI_IOV_RESOURCES + PCI_SRIOV_NUM_BARS - 1,
#endif

	/* resources assigned to buses behind the bridge */
#define PCI_BRIDGE_RESOURCE_NUM 4

	PCI_BRIDGE_RESOURCES,
	PCI_BRIDGE_RESOURCE_END = PCI_BRIDGE_RESOURCES +
				  PCI_BRIDGE_RESOURCE_NUM - 1,

	/* total resources associated with a PCI device */
	PCI_NUM_RESOURCES,

	/* preserve this for compatibility */
	DEVICE_COUNT_RESOURCE = PCI_NUM_RESOURCES,
};

typedef int __bitwise pci_power_t;

#define PCI_D0		((pci_power_t __force) 0)

#define PCI_D3hot	((pci_power_t __force) 3)
#define PCI_D3cold	((pci_power_t __force) 4)

extern const char *pci_power_names[];

/** The pci_channel state describes connectivity between the CPU and
 *  the pci device.  If some PCI bus between here and the pci device
 *  has crashed or locked up, this info is reflected here.
 */
typedef unsigned int __bitwise pci_channel_state_t;

enum pci_channel_state {
	/* I/O channel is in normal state */
	pci_channel_io_normal = (__force pci_channel_state_t) 1,

	/* I/O to channel is blocked */
	pci_channel_io_frozen = (__force pci_channel_state_t) 2,

	/* PCI card is dead */
	pci_channel_io_perm_failure = (__force pci_channel_state_t) 3,
};

typedef unsigned short __bitwise pci_dev_flags_t;

typedef unsigned short __bitwise pci_bus_flags_t;

struct pcie_link_state;
struct pci_vpd;
struct pci_sriov;
struct pci_ats;

/*
 * The pci_dev structure is used to describe PCI devices.
 */
struct pci_dev {
	struct list_head bus_list;	/* node in per-bus list */
	struct pci_bus	*bus;		/* bus this device is on */
	struct pci_bus	*subordinate;	/* bus this device bridges to */

	void		*sysdata;	/* hook for sys-specific extension */
	struct proc_dir_entry *procent;	/* device entry in /proc/bus/pci */
	struct pci_slot	*slot;		/* Physical slot this device is in */

	unsigned int	devfn;		/* encoded device & function index */
	unsigned short	vendor;
	unsigned short	device;
	unsigned short	subsystem_vendor;
	unsigned short	subsystem_device;
	unsigned int	class;		/* 3 bytes: (base,sub,prog-if) */
	u8		revision;	/* PCI revision, low byte of class word */
	u8		hdr_type;	/* PCI header type (`multi' flag masked out) */
	u8		pcie_cap;	/* PCI-E capability offset */
	u8		msi_cap;	/* MSI capability offset */
	u8		msix_cap;	/* MSI-X capability offset */
	u8		pcie_mpss:3;	/* PCI-E Max Payload Size Supported */
	u8		rom_base_reg;	/* which config register controls the ROM */
	u8		pin;  		/* which interrupt pin this device uses */
	u16		pcie_flags_reg;	/* cached PCI-E Capabilities Register */

	struct pci_driver *driver;	/* which driver has allocated this device */
	u64		dma_mask;	/* Mask of the bits of bus address this
					   device implements.  Normally this is
					   0xffffffff.  You only need to change
					   this if your device has broken DMA
					   or supports 64-bit transfers.  */

	struct device_dma_parameters dma_parms;

	pci_power_t     current_state;  /* Current operating state. In ACPI-speak,
					   this is D0-D3, D0 being fully functional,
					   and D3 being off. */
	u8		pm_cap;		/* PM capability offset */
	unsigned int	pme_support:5;	/* Bitmask of states from which PME#
					   can be generated */
	unsigned int	pme_interrupt:1;
	unsigned int	pme_poll:1;	/* Poll device's PME status bit */
	unsigned int	d1_support:1;	/* Low power state D1 is supported */
	unsigned int	d2_support:1;	/* Low power state D2 is supported */
	unsigned int	no_d1d2:1;	/* D1 and D2 are forbidden */
	unsigned int	no_d3cold:1;	/* D3cold is forbidden */
	unsigned int	d3cold_allowed:1;	/* D3cold is allowed by user */
	unsigned int	mmio_always_on:1;	/* disallow turning off io/mem
						   decoding during bar sizing */
	unsigned int	wakeup_prepared:1;
	unsigned int	runtime_d3cold:1;	/* whether go through runtime
						   D3cold, not set for devices
						   powered on/off by the
						   corresponding bridge */
	unsigned int	d3_delay;	/* D3->D0 transition time in ms */
	unsigned int	d3cold_delay;	/* D3cold->D0 transition time in ms */

#ifdef CONFIG_PCIEASPM
	struct pcie_link_state	*link_state;	/* ASPM link state. */
#endif

	pci_channel_state_t error_state;	/* current connectivity state */
	struct	device	dev;		/* Generic device interface */

	int		cfg_size;	/* Size of configuration space */

	/*
	 * Instead of touching interrupt line and base address registers
	 * directly, use the values stored here. They might be different!
	 */
	unsigned int	irq;
	struct resource resource[DEVICE_COUNT_RESOURCE]; /* I/O and memory regions + expansion ROMs */

	bool match_driver;		/* Skip attaching driver */
	/* These fields are used by common fixups */
	unsigned int	transparent:1;	/* Transparent PCI bridge */
	unsigned int	multifunction:1;/* Part of multi-function device */
	/* keep track of device state */
	unsigned int	is_added:1;
	unsigned int	is_busmaster:1; /* device is busmaster */
	unsigned int	no_msi:1;	/* device may not use msi */
	unsigned int	block_cfg_access:1;	/* config space access is blocked */
	unsigned int	broken_parity_status:1;	/* Device generates false positive parity */
	unsigned int	irq_reroute_variant:2;	/* device needs IRQ rerouting variant */
	unsigned int 	msi_enabled:1;
	unsigned int	msix_enabled:1;
	unsigned int	ari_enabled:1;	/* ARI forwarding */
	unsigned int	is_managed:1;
	unsigned int	is_pcie:1;	/* Obsolete. Will be removed.
					   Use pci_is_pcie() instead */
	unsigned int    needs_freset:1; /* Dev requires fundamental reset */
	unsigned int	state_saved:1;
	unsigned int	is_physfn:1;
	unsigned int	is_virtfn:1;
	unsigned int	reset_fn:1;
	unsigned int    is_hotplug_bridge:1;
	unsigned int    __aer_firmware_first_valid:1;
	unsigned int	__aer_firmware_first:1;
	unsigned int	broken_intx_masking:1;
	unsigned int	io_window_1k:1;	/* Intel P2P bridge 1K I/O windows */
	pci_dev_flags_t dev_flags;
	atomic_t	enable_cnt;	/* pci_enable_device has been called */

	u32		saved_config_space[16]; /* config space saved at suspend time */
	struct hlist_head saved_cap_space;
	struct bin_attribute *rom_attr; /* attribute descriptor for sysfs ROM entry */
	int rom_attr_enabled;		/* has display of the rom attribute been enabled? */
	struct bin_attribute *res_attr[DEVICE_COUNT_RESOURCE]; /* sysfs file for resources */
	struct bin_attribute *res_attr_wc[DEVICE_COUNT_RESOURCE]; /* sysfs file for WC mapping of resources */
#ifdef CONFIG_PCI_MSI
	struct list_head msi_list;
	struct kset *msi_kset;
#endif
	struct pci_vpd *vpd;
#ifdef CONFIG_PCI_ATS
	union {
		struct pci_sriov *sriov;	/* SR-IOV capability related */
		struct pci_dev *physfn;	/* the PF this VF is associated with */
	};
	struct pci_ats	*ats;	/* Address Translation Service */
#endif
	phys_addr_t rom; /* Physical address of ROM if it's not from the BAR */
	size_t romlen; /* Length of ROM if it's not from the BAR */
};

static inline int pci_channel_offline(struct pci_dev *pdev)
{
	return (pdev->error_state != pci_channel_io_normal);
}

struct pci_bus {
	struct list_head node;		/* node in list of buses */
	struct pci_bus	*parent;	/* parent bus this bridge is on */
	struct list_head children;	/* list of child buses */
	struct list_head devices;	/* list of devices on this bus */
	struct pci_dev	*self;		/* bridge device as seen by parent */
	struct list_head slots;		/* list of slots on this bus */
	struct resource *resource[PCI_BRIDGE_RESOURCE_NUM];
	struct list_head resources;	/* address space routed to this bus */
	struct resource busn_res;	/* bus numbers routed to this bus */

	struct pci_ops	*ops;		/* configuration access functions */
	void		*sysdata;	/* hook for sys-specific extension */
	struct proc_dir_entry *procdir;	/* directory entry in /proc/bus/pci */

	unsigned char	number;		/* bus number */
	unsigned char	primary;	/* number of primary bridge */
	unsigned char	max_bus_speed;	/* enum pci_bus_speed */
	unsigned char	cur_bus_speed;	/* enum pci_bus_speed */

	char		name[48];

	unsigned short  bridge_ctl;	/* manage NO_ISA/FBB/et al behaviors */
	pci_bus_flags_t bus_flags;	/* Inherited by child busses */
	struct device		*bridge;
	struct device		dev;
	struct bin_attribute	*legacy_io; /* legacy I/O for this bus */
	struct bin_attribute	*legacy_mem; /* legacy mem */
	unsigned int		is_added:1;
};

/*
 * Error values that may be returned by PCI functions.
 */
#define PCIBIOS_SUCCESSFUL		0x00
#define PCIBIOS_FUNC_NOT_SUPPORTED	0x81
#define PCIBIOS_BAD_VENDOR_ID		0x83
#define PCIBIOS_DEVICE_NOT_FOUND	0x86
#define PCIBIOS_BAD_REGISTER_NUMBER	0x87
#define PCIBIOS_SET_FAILED		0x88
#define PCIBIOS_BUFFER_TOO_SMALL	0x89

struct pci_ops {
	int (*read)(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 *val);
	int (*write)(struct pci_bus *bus, unsigned int devfn, int where, int size, u32 val);
};

struct pci_dynids {
	spinlock_t lock;            /* protects list, index */
	struct list_head list;      /* for IDs added at runtime */
};

typedef unsigned int __bitwise pci_ers_result_t;

enum pci_ers_result {
	/* no result/none/not supported in device driver */
	PCI_ERS_RESULT_NONE = (__force pci_ers_result_t) 1,

	/* Device driver can recover without slot reset */
	PCI_ERS_RESULT_CAN_RECOVER = (__force pci_ers_result_t) 2,

	/* Device driver wants slot to be reset. */
	PCI_ERS_RESULT_NEED_RESET = (__force pci_ers_result_t) 3,

	/* Device has completely failed, is unrecoverable */
	PCI_ERS_RESULT_DISCONNECT = (__force pci_ers_result_t) 4,

	/* Device driver is fully recovered and operational */
	PCI_ERS_RESULT_RECOVERED = (__force pci_ers_result_t) 5,

	/* No AER capabilities registered for the driver */
	PCI_ERS_RESULT_NO_AER_DRIVER = (__force pci_ers_result_t) 6,
};

struct pci_error_handlers {
	/* PCI bus error detected on this device */
	pci_ers_result_t (*error_detected)(struct pci_dev *dev,
					   enum pci_channel_state error);

	/* MMIO has been re-enabled, but not DMA */
	pci_ers_result_t (*mmio_enabled)(struct pci_dev *dev);

	/* PCI Express link has been reset */
	pci_ers_result_t (*link_reset)(struct pci_dev *dev);

	/* PCI slot has been reset */
	pci_ers_result_t (*slot_reset)(struct pci_dev *dev);

	/* Device driver may resume normal operations */
	void (*resume)(struct pci_dev *dev);
};

struct module;
struct pci_driver {
	struct list_head node;
	const char *name;
	const struct pci_device_id *id_table;	/* must be non-NULL for probe to be called */
	int  (*probe)  (struct pci_dev *dev, const struct pci_device_id *id);	/* New device inserted */
	void (*remove) (struct pci_dev *dev);	/* Device removed (NULL if not a hot-plug capable driver) */
	int  (*suspend) (struct pci_dev *dev, pm_message_t state);	/* Device suspended */
	int  (*suspend_late) (struct pci_dev *dev, pm_message_t state);
	int  (*resume_early) (struct pci_dev *dev);
	int  (*resume) (struct pci_dev *dev);	                /* Device woken up */
	void (*shutdown) (struct pci_dev *dev);
	int (*sriov_configure) (struct pci_dev *dev, int num_vfs); /* PF pdev */
	const struct pci_error_handlers *err_handler;
	struct device_driver	driver;
	struct pci_dynids dynids;
};

/**
 * DEFINE_PCI_DEVICE_TABLE - macro used to describe a pci device table
 * @_table: device table name
 *
 * This macro is used to create a struct pci_device_id array (a device table)
 * in a generic manner.
 */
#define DEFINE_PCI_DEVICE_TABLE(_table) \
	const struct pci_device_id _table[]

/**
 * PCI_DEVICE - macro used to describe a specific pci device
 * @vend: the 16 bit PCI Vendor ID
 * @dev: the 16 bit PCI Device ID
 *
 * This macro is used to create a struct pci_device_id that matches a
 * specific device.  The subvendor and subdevice fields will be set to
 * PCI_ANY_ID.
 */
#define PCI_DEVICE(vend,dev) \
	.vendor = (vend), .device = (dev), \
	.subvendor = PCI_ANY_ID, .subdevice = PCI_ANY_ID

struct pci_dev *pci_get_domain_bus_and_slot(int domain, unsigned int bus,
					    unsigned int devfn);

int pci_bus_read_config_byte(struct pci_bus *bus, unsigned int devfn,
			     int where, u8 *val);
int pci_bus_read_config_word(struct pci_bus *bus, unsigned int devfn,
			     int where, u16 *val);
int pci_bus_read_config_dword(struct pci_bus *bus, unsigned int devfn,
			      int where, u32 *val);
int pci_bus_write_config_byte(struct pci_bus *bus, unsigned int devfn,
			      int where, u8 val);
int pci_bus_write_config_word(struct pci_bus *bus, unsigned int devfn,
			      int where, u16 val);
int pci_bus_write_config_dword(struct pci_bus *bus, unsigned int devfn,
			       int where, u32 val);

static inline int pci_read_config_word(const struct pci_dev *dev, int where, u16 *val)
{
	return pci_bus_read_config_word(dev->bus, dev->devfn, where, val);
}

int pcie_capability_clear_and_set_word(struct pci_dev *dev, int pos,
				       u16 clear, u16 set);
int pcie_capability_clear_and_set_dword(struct pci_dev *dev, int pos,
					u32 clear, u32 set);

int __must_check pci_enable_device(struct pci_dev *dev);

int __must_check pci_enable_device_mem(struct pci_dev *dev);

void pci_disable_device(struct pci_dev *dev);

void pci_set_master(struct pci_dev *dev);

int __must_check pci_set_mwi(struct pci_dev *dev);

void pci_clear_mwi(struct pci_dev *dev);

int pcix_get_mmrbc(struct pci_dev *dev);
int pcix_set_mmrbc(struct pci_dev *dev, int mmrbc);

int pci_select_bars(struct pci_dev *dev, unsigned long flags);

int pci_save_state(struct pci_dev *dev);
void pci_restore_state(struct pci_dev *dev);

int pci_set_power_state(struct pci_dev *dev, pci_power_t state);

int __pci_enable_wake(struct pci_dev *dev, pci_power_t state,
		      bool runtime, bool enable);
int pci_wake_from_d3(struct pci_dev *dev, bool enable);

int pci_prepare_to_sleep(struct pci_dev *dev);

static inline int pci_enable_wake(struct pci_dev *dev, pci_power_t state,
				  bool enable)
{
	return __pci_enable_wake(dev, state, false, enable);
}

int pci_request_selected_regions(struct pci_dev *, int, const char *);

void pci_release_selected_regions(struct pci_dev *, int);

int __must_check __pci_register_driver(struct pci_driver *, struct module *,
				       const char *mod_name);

/*
 * pci_register_driver must be a macro so that KBUILD_MODNAME can be expanded
 */
#define pci_register_driver(driver)		\
	__pci_register_driver(driver, THIS_MODULE, KBUILD_MODNAME)

void pci_unregister_driver(struct pci_driver *dev);

#include <linux/pci-dma.h>
#include <linux/dmapool.h>

#include <asm/pci.h>

/* these helpers provide future and backwards compatibility
 * for accessing popular PCI BAR info */
#define pci_resource_start(dev, bar)	((dev)->resource[(bar)].start)
#define pci_resource_end(dev, bar)	((dev)->resource[(bar)].end)
#define pci_resource_flags(dev, bar)	((dev)->resource[(bar)].flags)
#define pci_resource_len(dev,bar) \
	((pci_resource_start((dev), (bar)) == 0 &&	\
	  pci_resource_end((dev), (bar)) ==		\
	  pci_resource_start((dev), (bar))) ? 0 :	\
							\
	 (pci_resource_end((dev), (bar)) -		\
	  pci_resource_start((dev), (bar)) + 1))

/* Similar to the helpers above, these manipulate per-pci_dev
 * driver-specific data.  They are really just a wrapper around
 * the generic device structure functions of these calls.
 */
static inline void *pci_get_drvdata(struct pci_dev *pdev)
{
	return dev_get_drvdata(&pdev->dev);
}

static inline void pci_set_drvdata(struct pci_dev *pdev, void *data)
{
	dev_set_drvdata(&pdev->dev, data);
}

/* If you want to know what to call your pci_dev, ask this function.
 * Again, it's a wrapper around the generic device.
 */
static inline const char *pci_name(const struct pci_dev *pdev)
{
	return dev_name(&pdev->dev);
}

void __iomem *pci_ioremap_bar(struct pci_dev *pdev, int bar);

/**
 * pci_pcie_cap - get the saved PCIe capability offset
 * @dev: PCI device
 *
 * PCIe capability offset is calculated at PCI device initialization
 * time and saved in the data structure. This function returns saved
 * PCIe capability offset. Using this instead of pci_find_capability()
 * reduces unnecessary search in the PCI configuration space. If you
 * need to calculate PCIe capability offset from raw device for some
 * reasons, please use pci_find_capability() instead.
 */
static inline int pci_pcie_cap(struct pci_dev *dev)
{
	return dev->pcie_cap;
}

/**
 * pcie_caps_reg - get the PCIe Capabilities Register
 * @dev: PCI device
 */
static inline u16 pcie_caps_reg(const struct pci_dev *dev)
{
	return dev->pcie_flags_reg;
}

#define PCI_VPD_SRDT_LEN_MASK		0x07

#endif /* ! __LINUX_PCI_H__ */
