#ifndef __LINUX_STAT_H__
#define __LINUX_STAT_H__

#include <asm/stat.h>
#include <uapi/linux/stat.h>

#define S_IRUGO		(S_IRUSR|S_IRGRP|S_IROTH)

#define S_IXUGO		(S_IXUSR|S_IXGRP|S_IXOTH)

#include <linux/types.h>
#include <linux/time.h>
#include <linux/uidgid.h>

struct kstat {
	u64		ino;
	dev_t		dev;
	umode_t		mode;
	unsigned int	nlink;
	kuid_t		uid;
	kgid_t		gid;
	dev_t		rdev;
	loff_t		size;
	struct timespec  atime;
	struct timespec	mtime;
	struct timespec	ctime;
	unsigned long	blksize;
	unsigned long long	blocks;
};

#endif /* ! __LINUX_STAT_H__ */
