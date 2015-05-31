#ifndef __LINUX_PATH_H__
#define __LINUX_PATH_H__

struct dentry;
struct vfsmount;

struct path {
	struct vfsmount *mnt;
	struct dentry *dentry;
};

#endif /* ! __LINUX_PATH_H__ */
