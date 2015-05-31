#ifndef __LINUX_PM_H__
#define __LINUX_PM_H__

#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/timer.h>
#include <linux/completion.h>

struct device; /* we have a circular dep with device.h */

struct device;

typedef struct pm_message {
	int event;
} pm_message_t;

struct dev_pm_ops {
	int (*prepare)(struct device *dev);
	void (*complete)(struct device *dev);
	int (*suspend)(struct device *dev);
	int (*resume)(struct device *dev);
	int (*freeze)(struct device *dev);
	int (*thaw)(struct device *dev);
	int (*poweroff)(struct device *dev);
	int (*restore)(struct device *dev);
	int (*suspend_late)(struct device *dev);
	int (*resume_early)(struct device *dev);
	int (*freeze_late)(struct device *dev);
	int (*thaw_early)(struct device *dev);
	int (*poweroff_late)(struct device *dev);
	int (*restore_early)(struct device *dev);
	int (*suspend_noirq)(struct device *dev);
	int (*resume_noirq)(struct device *dev);
	int (*freeze_noirq)(struct device *dev);
	int (*thaw_noirq)(struct device *dev);
	int (*poweroff_noirq)(struct device *dev);
	int (*restore_noirq)(struct device *dev);
	int (*runtime_suspend)(struct device *dev);
	int (*runtime_resume)(struct device *dev);
	int (*runtime_idle)(struct device *dev);
};

struct wakeup_source;

struct pm_subsys_data {
	spinlock_t lock;
	unsigned int refcount;
#ifdef CONFIG_PM_CLK
	struct list_head clock_list;
#endif
#ifdef CONFIG_PM_GENERIC_DOMAINS
	struct pm_domain_data *domain_data;
#endif
};

struct dev_pm_info {
	pm_message_t		power_state;
	unsigned int		can_wakeup:1;
	unsigned int		async_suspend:1;
	bool			is_prepared:1;	/* Owned by the PM core */
	bool			is_suspended:1;	/* Ditto */
	bool			ignore_children:1;
	bool			early_init:1;	/* Owned by the PM core */
	spinlock_t		lock;
#ifdef CONFIG_PM_SLEEP
	struct list_head	entry;
	struct completion	completion;
	struct wakeup_source	*wakeup;
	bool			wakeup_path:1;
	bool			syscore:1;
#else
	unsigned int		should_wakeup:1;
#endif
#ifdef CONFIG_PM_RUNTIME
	struct timer_list	suspend_timer;
	unsigned long		timer_expires;
	struct work_struct	work;
	wait_queue_head_t	wait_queue;
	atomic_t		usage_count;
	atomic_t		child_count;
	unsigned int		disable_depth:3;
	unsigned int		idle_notification:1;
	unsigned int		request_pending:1;
	unsigned int		deferred_resume:1;
	unsigned int		run_wake:1;
	unsigned int		runtime_auto:1;
	unsigned int		no_callbacks:1;
	unsigned int		irq_safe:1;
	unsigned int		use_autosuspend:1;
	unsigned int		timer_autosuspends:1;
	unsigned int		memalloc_noio:1;
	enum rpm_request	request;
	enum rpm_status		runtime_status;
	int			runtime_error;
	int			autosuspend_delay;
	unsigned long		last_busy;
	unsigned long		active_jiffies;
	unsigned long		suspended_jiffies;
	unsigned long		accounting_timestamp;
#endif
	struct pm_subsys_data	*subsys_data;  /* Owned by the subsystem. */
	struct dev_pm_qos	*qos;
};

/*
 * Power domains provide callbacks that are executed during system suspend,
 * hibernation, system resume and during runtime PM transitions along with
 * subsystem-level and driver-level callbacks.
 */
struct dev_pm_domain {
	struct dev_pm_ops	ops;
};

#endif /* ! __LINUX_PM_H__ */
