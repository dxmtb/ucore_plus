#ifndef __LINUX_UIDGID_H__
#define __LINUX_UIDGID_H__

/*
 * A set of types for the internal kernel types representing uids and gids.
 *
 * The types defined in this header allow distinguishing which uids and gids in
 * the kernel are values used by userspace and which uid and gid values are
 * the internal kernel values.  With the addition of user namespaces the values
 * can be different.  Using the type system makes it possible for the compiler
 * to detect when we overlook these differences.
 *
 */
#include <linux/types.h>
#include <linux/highuid.h>

struct user_namespace;
extern struct user_namespace init_user_ns;

typedef uid_t kuid_t;
typedef gid_t kgid_t;

static inline uid_t __kuid_val(kuid_t uid)
{
	return uid;
}

static inline gid_t __kgid_val(kgid_t gid)
{
	return gid;
}

#define KUIDT_INIT(value) ((kuid_t) value )
#define KGIDT_INIT(value) ((kgid_t) value )

#define INVALID_UID KUIDT_INIT(-1)
#define INVALID_GID KGIDT_INIT(-1)

static inline bool uid_eq(kuid_t left, kuid_t right)
{
	return __kuid_val(left) == __kuid_val(right);
}

static inline bool gid_eq(kgid_t left, kgid_t right)
{
	return __kgid_val(left) == __kgid_val(right);
}

static inline kuid_t make_kuid(struct user_namespace *from, uid_t uid)
{
	return KUIDT_INIT(uid);
}

static inline kgid_t make_kgid(struct user_namespace *from, gid_t gid)
{
	return KGIDT_INIT(gid);
}

static inline uid_t from_kuid(struct user_namespace *to, kuid_t kuid)
{
	return __kuid_val(kuid);
}

static inline gid_t from_kgid(struct user_namespace *to, kgid_t kgid)
{
	return __kgid_val(kgid);
}

static inline uid_t from_kuid_munged(struct user_namespace *to, kuid_t kuid)
{
	uid_t uid = from_kuid(to, kuid);
	if (uid == (uid_t)-1)
		uid = overflowuid;
	return uid;
}

static inline gid_t from_kgid_munged(struct user_namespace *to, kgid_t kgid)
{
	gid_t gid = from_kgid(to, kgid);
	if (gid == (gid_t)-1)
		gid = overflowgid;
	return gid;
}

#endif /* ! __LINUX_UIDGID_H__ */
