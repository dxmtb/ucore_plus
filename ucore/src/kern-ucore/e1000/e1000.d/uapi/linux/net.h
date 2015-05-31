#ifndef __UAPI_LINUX_NET_H__
#define __UAPI_LINUX_NET_H__

#include <linux/socket.h>
#include <asm/socket.h>

typedef enum {
	SS_FREE = 0,			/* not allocated		*/
	SS_UNCONNECTED,			/* unconnected to any socket	*/
	SS_CONNECTING,			/* in process of connecting	*/
	SS_CONNECTED,			/* connected to socket		*/
	SS_DISCONNECTING		/* in process of disconnecting	*/
} socket_state;

#endif /* ! __UAPI_LINUX_NET_H__ */
