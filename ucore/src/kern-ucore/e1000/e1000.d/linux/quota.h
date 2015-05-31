#ifndef __LINUX_QUOTA_H__
#define __LINUX_QUOTA_H__

#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/rwsem.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/percpu_counter.h>

#include <linux/dqblk_xfs.h>
#include <linux/dqblk_v1.h>
#include <linux/dqblk_v2.h>

#include <linux/atomic.h>
#include <linux/uidgid.h>
#include <linux/projid.h>
#include <uapi/linux/quota.h>

#undef USRQUOTA
#undef GRPQUOTA
enum quota_type {
	USRQUOTA = 0,		/* element used for user quotas */
	GRPQUOTA = 1,		/* element used for group quotas */
	PRJQUOTA = 2,		/* element used for project quotas */
};

typedef long long qsize_t;	/* Type in which we store sizes */

struct kqid {			/* Type in which we store the quota identifier */
	union {
		kuid_t uid;
		kgid_t gid;
		kprojid_t projid;
	};
	enum quota_type type;  /* USRQUOTA (uid) or GRPQUOTA (gid) or PRJQUOTA (projid) */
};

/*
 * Data for one user/group kept in memory
 */
struct mem_dqblk {
	qsize_t dqb_bhardlimit;	/* absolute limit on disk blks alloc */
	qsize_t dqb_bsoftlimit;	/* preferred limit on disk blks */
	qsize_t dqb_curspace;	/* current used space */
	qsize_t dqb_rsvspace;   /* current reserved space for delalloc*/
	qsize_t dqb_ihardlimit;	/* absolute limit on allocated inodes */
	qsize_t dqb_isoftlimit;	/* preferred inode limit */
	qsize_t dqb_curinodes;	/* current # allocated inodes */
	time_t dqb_btime;	/* time limit for excessive disk use */
	time_t dqb_itime;	/* time limit for excessive inode use */
};

/*
 * Data for one quotafile kept in memory
 */
struct quota_format_type;

struct mem_dqinfo {
	struct quota_format_type *dqi_format;
	int dqi_fmt_id;		/* Id of the dqi_format - used when turning
				 * quotas on after remount RW */
	struct list_head dqi_dirty_list;	/* List of dirty dquots */
	unsigned long dqi_flags;
	unsigned int dqi_bgrace;
	unsigned int dqi_igrace;
	qsize_t dqi_maxblimit;
	qsize_t dqi_maxilimit;
	void *dqi_priv;
};

struct super_block;

#define DQF_INFO_DIRTY_B	31

enum {
	DQST_LOOKUPS,
	DQST_DROPS,
	DQST_READS,
	DQST_WRITES,
	DQST_CACHE_HITS,
	DQST_ALLOC_DQUOTS,
	DQST_FREE_DQUOTS,
	DQST_SYNCS,
	_DQST_DQSTAT_LAST
};

struct dqstats {
	int stat[_DQST_DQSTAT_LAST];
	struct percpu_counter counter[_DQST_DQSTAT_LAST];
};

extern struct dqstats dqstats;

struct dquot {
	struct hlist_node dq_hash;	/* Hash list in memory */
	struct list_head dq_inuse;	/* List of all quotas */
	struct list_head dq_free;	/* Free list element */
	struct list_head dq_dirty;	/* List of dirty dquots */
	struct mutex dq_lock;		/* dquot IO lock */
	atomic_t dq_count;		/* Use count */
	wait_queue_head_t dq_wait_unused;	/* Wait queue for dquot to become unused */
	struct super_block *dq_sb;	/* superblock this applies to */
	struct kqid dq_id;		/* ID this applies to (uid, gid, projid) */
	loff_t dq_off;			/* Offset of dquot on disk */
	unsigned long dq_flags;		/* See DQ_* */
	struct mem_dqblk dq_dqb;	/* Diskquota usage */
};

struct quota_format_ops {
	int (*check_quota_file)(struct super_block *sb, int type);	/* Detect whether file is in our format */
	int (*read_file_info)(struct super_block *sb, int type);	/* Read main info about file - called on quotaon() */
	int (*write_file_info)(struct super_block *sb, int type);	/* Write main info about file */
	int (*free_file_info)(struct super_block *sb, int type);	/* Called on quotaoff() */
	int (*read_dqblk)(struct dquot *dquot);		/* Read structure for one user */
	int (*commit_dqblk)(struct dquot *dquot);	/* Write structure for one user */
	int (*release_dqblk)(struct dquot *dquot);	/* Called when last reference to dquot is being dropped */
};

struct dquot_operations {
	int (*write_dquot) (struct dquot *);		/* Ordinary dquot write */
	struct dquot *(*alloc_dquot)(struct super_block *, int);	/* Allocate memory for new dquot */
	void (*destroy_dquot)(struct dquot *);		/* Free memory for dquot */
	int (*acquire_dquot) (struct dquot *);		/* Quota is going to be created on disk */
	int (*release_dquot) (struct dquot *);		/* Quota is going to be deleted from disk */
	int (*mark_dirty) (struct dquot *);		/* Dquot is marked dirty */
	int (*write_info) (struct super_block *, int);	/* Write of quota "superblock" */
	/* get reserved quota for delayed alloc, value returned is managed by
	 * quota code only */
	qsize_t *(*get_reserved_space) (struct inode *);
};

struct path;

struct quotactl_ops {
	int (*quota_on)(struct super_block *, int, int, struct path *);
	int (*quota_on_meta)(struct super_block *, int, int);
	int (*quota_off)(struct super_block *, int);
	int (*quota_sync)(struct super_block *, int);
	int (*get_info)(struct super_block *, int, struct if_dqinfo *);
	int (*set_info)(struct super_block *, int, struct if_dqinfo *);
	int (*get_dqblk)(struct super_block *, struct kqid, struct fs_disk_quota *);
	int (*set_dqblk)(struct super_block *, struct kqid, struct fs_disk_quota *);
	int (*get_xstate)(struct super_block *, struct fs_quota_stat *);
	int (*set_xstate)(struct super_block *, unsigned int, int);
};

struct quota_format_type {
	int qf_fmt_id;	/* Quota format id */
	const struct quota_format_ops *qf_ops;	/* Operations of format */
	struct module *qf_owner;		/* Module implementing quota format */
	struct quota_format_type *qf_next;
};

#define DQUOT_USAGE_ENABLED	(1 << _DQUOT_USAGE_ENABLED)
#define DQUOT_LIMITS_ENABLED	(1 << _DQUOT_LIMITS_ENABLED)
#define DQUOT_SUSPENDED		(1 << _DQUOT_SUSPENDED)
#define DQUOT_STATE_FLAGS	(DQUOT_USAGE_ENABLED | DQUOT_LIMITS_ENABLED | \
				 DQUOT_SUSPENDED)

struct quota_info {
	unsigned int flags;			/* Flags for diskquotas on this device */
	struct mutex dqio_mutex;		/* lock device while I/O in progress */
	struct mutex dqonoff_mutex;		/* Serialize quotaon & quotaoff */
	struct rw_semaphore dqptr_sem;		/* serialize ops using quota_info struct, pointers from inode to dquots */
	struct inode *files[MAXQUOTAS];		/* inodes of quotafiles */
	struct mem_dqinfo info[MAXQUOTAS];	/* Information for each quota type */
	const struct quota_format_ops *ops[MAXQUOTAS];	/* Operations for each type */
};

#endif /* ! __LINUX_QUOTA_H__ */
