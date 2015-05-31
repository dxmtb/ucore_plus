#ifndef __UAPI_ASM_GENERIC_POLL_H__
#define __UAPI_ASM_GENERIC_POLL_H__

#define POLLIN		0x0001

#define POLLRDNORM	0x0040

struct pollfd {
	int fd;
	short events;
	short revents;
};

#endif /* ! __UAPI_ASM_GENERIC_POLL_H__ */
