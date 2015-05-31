#ifndef __NET_SCM_H__
#define __NET_SCM_H__

#include <linux/limits.h>
#include <linux/net.h>
#include <linux/security.h>
#include <linux/pid.h>
#include <linux/nsproxy.h>

/* Well, we should have at least one descriptor open
 * to accept passed FDs 8)
 */
#define SCM_MAX_FD	253

struct scm_creds {
	u32	pid;
	kuid_t	uid;
	kgid_t	gid;
};

struct scm_fp_list {
	short			count;
	short			max;
	struct file		*fp[SCM_MAX_FD];
};

struct scm_cookie {
	struct pid		*pid;		/* Skb credentials */
	struct scm_fp_list	*fp;		/* Passed files		*/
	struct scm_creds	creds;		/* Skb credentials	*/
#ifdef CONFIG_SECURITY_NETWORK
	u32			secid;		/* Passed security ID 	*/
#endif
};

extern void scm_detach_fds(struct msghdr *msg, struct scm_cookie *scm);

extern int __scm_send(struct socket *sock, struct msghdr *msg, struct scm_cookie *scm);
extern void __scm_destroy(struct scm_cookie *scm);

static __inline__ void unix_get_peersec_dgram(struct socket *sock, struct scm_cookie *scm)
{
	security_socket_getpeersec_dgram(sock, NULL, &scm->secid);
}

static __inline__ void scm_set_cred(struct scm_cookie *scm,
				    struct pid *pid, kuid_t uid, kgid_t gid)
{
	scm->pid  = get_pid(pid);
	scm->creds.pid = pid_vnr(pid);
	scm->creds.uid = uid;
	scm->creds.gid = gid;
}

static __inline__ void scm_destroy_cred(struct scm_cookie *scm)
{
	put_pid(scm->pid);
	scm->pid  = NULL;
}

static __inline__ void scm_destroy(struct scm_cookie *scm)
{
	scm_destroy_cred(scm);
	if (scm->fp)
		__scm_destroy(scm);
}

static inline void scm_passec(struct socket *sock, struct msghdr *msg, struct scm_cookie *scm)
{
	char *secdata;
	u32 seclen;
	int err;

	if (test_bit(SOCK_PASSSEC, &sock->flags)) {
		err = security_secid_to_secctx(scm->secid, &secdata, &seclen);

		if (!err) {
			put_cmsg(msg, SOL_SOCKET, SCM_SECURITY, seclen, secdata);
			security_release_secctx(secdata, seclen);
		}
	}
}

#endif /* ! __NET_SCM_H__ */
