#ifndef __UAPI_LINUX_DQBLK_XFS_H__
#define __UAPI_LINUX_DQBLK_XFS_H__

#include <linux/types.h>

typedef struct fs_disk_quota {
	__s8		d_version;	/* version of this structure */
	__s8		d_flags;	/* FS_{USER,PROJ,GROUP}_QUOTA */
	__u16		d_fieldmask;	/* field specifier */
	__u32		d_id;		/* user, project, or group ID */
	__u64		d_blk_hardlimit;/* absolute limit on disk blks */
	__u64		d_blk_softlimit;/* preferred limit on disk blks */
	__u64		d_ino_hardlimit;/* maximum # allocated inodes */
	__u64		d_ino_softlimit;/* preferred inode limit */
	__u64		d_bcount;	/* # disk blocks owned by the user */
	__u64		d_icount;	/* # inodes owned by the user */
	__s32		d_itimer;	/* zero if within inode limits */
					/* if not, we refuse service */
	__s32		d_btimer;	/* similar to above; for disk blocks */
	__u16	  	d_iwarns;       /* # warnings issued wrt num inodes */
	__u16	  	d_bwarns;       /* # warnings issued wrt disk blocks */
	__s32		d_padding2;	/* padding2 - for future use */
	__u64		d_rtb_hardlimit;/* absolute limit on realtime blks */
	__u64		d_rtb_softlimit;/* preferred limit on RT disk blks */
	__u64		d_rtbcount;	/* # realtime blocks owned */
	__s32		d_rtbtimer;	/* similar to above; for RT disk blks */
	__u16	  	d_rtbwarns;     /* # warnings issued wrt RT disk blks */
	__s16		d_padding3;	/* padding3 - for future use */
	char		d_padding4[8];	/* yet more padding */
} fs_disk_quota_t;

/*
 * Some basic information about 'quota files'.
 */
typedef struct fs_qfilestat {
	__u64		qfs_ino;	/* inode number */
	__u64		qfs_nblks;	/* number of BBs 512-byte-blks */
	__u32		qfs_nextents;	/* number of extents */
} fs_qfilestat_t;

typedef struct fs_quota_stat {
	__s8		qs_version;	/* version number for future changes */
	__u16		qs_flags;	/* FS_QUOTA_{U,P,G}DQ_{ACCT,ENFD} */
	__s8		qs_pad;		/* unused */
	fs_qfilestat_t	qs_uquota;	/* user quota storage information */
	fs_qfilestat_t	qs_gquota;	/* group quota storage information */
	__u32		qs_incoredqs;	/* number of dquots incore */
	__s32		qs_btimelimit;  /* limit for blks timer */
	__s32		qs_itimelimit;  /* limit for inodes timer */
	__s32		qs_rtbtimelimit;/* limit for rt blks timer */
	__u16		qs_bwarnlimit;	/* limit for num warnings */
	__u16		qs_iwarnlimit;	/* limit for num warnings */
} fs_quota_stat_t;

#endif /* ! __UAPI_LINUX_DQBLK_XFS_H__ */
