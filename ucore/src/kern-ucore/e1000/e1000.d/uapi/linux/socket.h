#ifndef __UAPI_LINUX_SOCKET_H__
#define __UAPI_LINUX_SOCKET_H__

/*
 * Desired design of maximum size and alignment (see RFC2553)
 */
#define _K_SS_MAXSIZE	128	/* Implementation specific max size */
#define _K_SS_ALIGNSIZE	(__alignof__ (struct sockaddr *))

typedef unsigned short __kernel_sa_family_t;

#endif /* ! __UAPI_LINUX_SOCKET_H__ */
