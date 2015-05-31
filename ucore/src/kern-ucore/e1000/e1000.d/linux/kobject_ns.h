#ifndef __LINUX_KOBJECT_NS_H__
#define __LINUX_KOBJECT_NS_H__

struct sock;
struct kobject;

/*
 * Namespace types which are used to tag kobjects and sysfs entries.
 * Network namespace will likely be the first.
 */
enum kobj_ns_type {
	KOBJ_NS_TYPE_NONE = 0,
	KOBJ_NS_TYPE_NET,
	KOBJ_NS_TYPES
};

/*
 * Callbacks so sysfs can determine namespaces
 *   @grab_current_ns: return a new reference to calling task's namespace
 *   @netlink_ns: return namespace to which a sock belongs (right?)
 *   @initial_ns: return the initial namespace (i.e. init_net_ns)
 *   @drop_ns: drops a reference to namespace
 */
struct kobj_ns_type_operations {
	enum kobj_ns_type type;
	void *(*grab_current_ns)(void);
	const void *(*netlink_ns)(struct sock *sk);
	const void *(*initial_ns)(void);
	void (*drop_ns)(void *);
};

#endif /* ! __LINUX_KOBJECT_NS_H__ */
