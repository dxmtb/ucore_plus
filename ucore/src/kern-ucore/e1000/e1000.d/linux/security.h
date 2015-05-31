#ifndef __LINUX_SECURITY_H__
#define __LINUX_SECURITY_H__

#include <linux/key.h>
#include <linux/capability.h>
#include <linux/slab.h>
#include <linux/err.h>

struct cred;
struct rlimit;
struct siginfo;

struct audit_context;
struct super_block;
struct inode;
struct dentry;
struct file;
struct vfsmount;
struct path;
struct qstr;
struct nameidata;
struct iattr;
struct fown_struct;
struct file_operations;

struct mm_struct;

#define SECURITY_NAME_MAX	10

struct ctl_table;

struct user_namespace;

struct msghdr;
struct sk_buff;
struct sock;
struct sockaddr;
struct socket;
struct flowi;
struct dst_entry;

struct xfrm_policy;
struct xfrm_state;

struct seq_file;

struct request_sock;

int security_secid_to_secctx(u32 secid, char **secdata, u32 *seclen);

void security_release_secctx(char *secdata, u32 seclen);

int security_socket_getpeersec_dgram(struct socket *sock, struct sk_buff *skb, u32 *secid);

void security_sock_graft(struct sock*sk, struct socket *parent);

#endif /* ! __LINUX_SECURITY_H__ */
