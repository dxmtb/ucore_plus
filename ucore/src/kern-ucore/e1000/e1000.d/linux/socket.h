#ifndef __LINUX_SOCKET_H__
#define __LINUX_SOCKET_H__

#include <asm/socket.h>			/* arch-dependent defines	*/
#include <linux/sockios.h>		/* the SIOCxxx I/O controls	*/
#include <linux/uio.h>			/* iovec support		*/
#include <linux/types.h>		/* pid_t			*/
#include <linux/compiler.h>		/* __user			*/
#include <uapi/linux/socket.h>

struct pid;
struct cred;

struct seq_file;

typedef __kernel_sa_family_t	sa_family_t;

struct sockaddr {
	sa_family_t	sa_family;	/* address family, AF_xxx	*/
	char		sa_data[14];	/* 14 bytes of protocol address	*/
};

#define sockaddr_storage __kernel_sockaddr_storage

struct msghdr {
	void	*	msg_name;	/* Socket name			*/
	int		msg_namelen;	/* Length of name		*/
	struct iovec *	msg_iov;	/* Data blocks			*/
	__kernel_size_t	msg_iovlen;	/* Number of blocks		*/
	void 	*	msg_control;	/* Per protocol magic (eg BSD file descriptor passing) */
	__kernel_size_t	msg_controllen;	/* Length of cmsg list */
	unsigned int	msg_flags;
};

struct cmsghdr {
	__kernel_size_t	cmsg_len;	/* data byte count, including hdr */
        int		cmsg_level;	/* originating protocol */
        int		cmsg_type;	/* protocol-specific type */
};

#define CMSG_ALIGN(len) ( ((len)+sizeof(long)-1) & ~(sizeof(long)-1) )

static inline struct cmsghdr * __cmsg_nxthdr(void *__ctl, __kernel_size_t __size,
					       struct cmsghdr *__cmsg)
{
	struct cmsghdr * __ptr;

	__ptr = (struct cmsghdr*)(((unsigned char *) __cmsg) +  CMSG_ALIGN(__cmsg->cmsg_len));
	if ((unsigned long)((char*)(__ptr+1) - (char *) __ctl) > __size)
		return (struct cmsghdr *)0;

	return __ptr;
}

#define SCM_CREDENTIALS 0x02		/* rw: struct ucred		*/
#define SCM_SECURITY	0x03		/* rw: security label		*/

#define AF_UNSPEC	0

#define AF_INET		2	/* Internet IP Protocol 	*/

#define AF_INET6	10	/* IP version 6			*/

#define AF_DECnet	12	/* Reserved for DECnet project	*/

#define PF_INET6	AF_INET6

#define MSG_PEEK	2

#define MSG_CTRUNC	8

#define MSG_DONTWAIT	0x40	/* Nonblocking io		 */

extern int put_cmsg(struct msghdr*, int level, int type, int len, void *data);

struct timespec;

#endif /* ! __LINUX_SOCKET_H__ */
