#ifndef __LINUX_KEY_H__
#define __LINUX_KEY_H__

#include <linux/types.h>
#include <linux/list.h>
#include <linux/rbtree.h>
#include <linux/rcupdate.h>
#include <linux/sysctl.h>
#include <linux/rwsem.h>
#include <linux/atomic.h>

#include <linux/uidgid.h>

typedef int32_t key_serial_t;

typedef uint32_t key_perm_t;

struct key;

struct seq_file;
struct user_struct;
struct signal_struct;
struct cred;

struct key_type;

struct keyring_list;

/*
 * key reference with possession attribute handling
 *
 * NOTE! key_ref_t is a typedef'd pointer to a type that is not actually
 * defined. This is because we abuse the bottom bit of the reference to carry a
 * flag to indicate whether the calling process possesses that key in one of
 * its keyrings.
 *
 * the key_ref_t has been made a separate type so that the compiler can reject
 * attempts to dereference it without proper conversion.
 *
 * the three functions are used to assemble and disassemble references
 */
typedef struct __key_reference_with_attributes *key_ref_t;

static inline struct key *key_ref_to_ptr(const key_ref_t key_ref)
{
	return (struct key *) ((unsigned long) key_ref & ~1UL);
}

/*
 * authentication token / access credential / keyring
 * - types of key include:
 *   - keyrings
 *   - disk encryption IDs
 *   - Kerberos TGTs and tickets
 */
struct key {
	atomic_t		usage;		/* number of references */
	key_serial_t		serial;		/* key serial number */
	union {
		struct list_head graveyard_link;
		struct rb_node	serial_node;
	};
	struct key_type		*type;		/* type of key */
	struct rw_semaphore	sem;		/* change vs change sem */
	struct key_user		*user;		/* owner of this key */
	void			*security;	/* security data for this key */
	union {
		time_t		expiry;		/* time at which key expires (or 0) */
		time_t		revoked_at;	/* time at which key was revoked */
	};
	time_t			last_used_at;	/* last time used for LRU keyring discard */
	kuid_t			uid;
	kgid_t			gid;
	key_perm_t		perm;		/* access permissions */
	unsigned short		quotalen;	/* length added to quota */
	unsigned short		datalen;	/* payload data length
						 * - may not match RCU dereferenced payload
						 * - payload should contain own length
						 */

#ifdef KEY_DEBUGGING
	unsigned		magic;
#define KEY_DEBUG_MAGIC		0x18273645u
#define KEY_DEBUG_MAGIC_X	0xf8e9dacbu
#endif

	unsigned long		flags;		/* status flags (change with bitops) */
#define KEY_FLAG_INSTANTIATED	0	/* set if key has been instantiated */
#define KEY_FLAG_DEAD		1	/* set if key type has been deleted */
#define KEY_FLAG_REVOKED	2	/* set if key had been revoked */
#define KEY_FLAG_IN_QUOTA	3	/* set if key consumes quota */
#define KEY_FLAG_USER_CONSTRUCT	4	/* set if key is being constructed in userspace */
#define KEY_FLAG_NEGATIVE	5	/* set if key is negative */
#define KEY_FLAG_ROOT_CAN_CLEAR	6	/* set if key can be cleared by root without permission */
#define KEY_FLAG_INVALIDATED	7	/* set if key has been invalidated */

	/* the description string
	 * - this is used to match a key against search criteria
	 * - this should be a printable string
	 * - eg: for krb5 AFS, this might be "afs@REDHAT.COM"
	 */
	char			*description;

	/* type specific data
	 * - this is used by the keyring type to index the name
	 */
	union {
		struct list_head	link;
		unsigned long		x[2];
		void			*p[2];
		int			reject_error;
	} type_data;

	/* key data
	 * - this is used to hold the data actually used in cryptography or
	 *   whatever
	 */
	union {
		unsigned long		value;
		void __rcu		*rcudata;
		void			*data;
		struct keyring_list __rcu *subscriptions;
	} payload;
};

extern void key_put(struct key *key);

#endif /* ! __LINUX_KEY_H__ */
