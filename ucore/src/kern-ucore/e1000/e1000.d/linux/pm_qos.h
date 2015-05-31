#ifndef __LINUX_PM_QOS_H__
#define __LINUX_PM_QOS_H__

/* interface for the pm_qos_power infrastructure of the linux kernel.
 *
 * Mark Gross <mgross@linux.intel.com>
 */
#include <linux/plist.h>
#include <linux/notifier.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/workqueue.h>

struct pm_qos_request {
	struct plist_node node;
	int pm_qos_class;
	struct delayed_work work; /* for pm_qos_update_request_timeout */
};

struct pm_qos_flags_request {
	struct list_head node;
	s32 flags;	/* Do not change to 64 bit */
};

enum dev_pm_qos_req_type {
	DEV_PM_QOS_LATENCY = 1,
	DEV_PM_QOS_FLAGS,
};

struct dev_pm_qos_request {
	enum dev_pm_qos_req_type type;
	union {
		struct plist_node pnode;
		struct pm_qos_flags_request flr;
	} data;
	struct device *dev;
};

enum pm_qos_type {
	PM_QOS_UNITIALIZED,
	PM_QOS_MAX,		/* return the largest value */
	PM_QOS_MIN		/* return the smallest value */
};

/*
 * Note: The lockless read path depends on the CPU accessing target_value
 * or effective_flags atomically.  Atomic access is only guaranteed on all CPU
 * types linux supports for 32 bit quantites
 */
struct pm_qos_constraints {
	struct plist_head list;
	s32 target_value;	/* Do not change to 64 bit */
	s32 default_value;
	enum pm_qos_type type;
	struct blocking_notifier_head *notifiers;
};

struct pm_qos_flags {
	struct list_head list;
	s32 effective_flags;	/* Do not change to 64 bit */
};

struct dev_pm_qos {
	struct pm_qos_constraints latency;
	struct pm_qos_flags flags;
	struct dev_pm_qos_request *latency_req;
	struct dev_pm_qos_request *flags_req;
};

#endif /* ! __LINUX_PM_QOS_H__ */
