#ifndef __LINUX_NFS_FS_I_H__
#define __LINUX_NFS_FS_I_H__

struct nlm_lockowner;

/*
 * NFS lock info
 */
struct nfs_lock_info {
	u32		state;
	struct nlm_lockowner *owner;
	struct list_head list;
};

struct nfs4_lock_state;
struct nfs4_lock_info {
	struct nfs4_lock_state *owner;
};

#endif /* ! __LINUX_NFS_FS_I_H__ */
