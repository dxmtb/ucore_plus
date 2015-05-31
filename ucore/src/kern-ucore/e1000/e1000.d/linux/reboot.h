#ifndef __LINUX_REBOOT_H__
#define __LINUX_REBOOT_H__

#include <linux/notifier.h>
#include <uapi/linux/reboot.h>

struct pt_regs;

#define POWEROFF_CMD_PATH_LEN	256

#include <asm/emergency-restart.h>

#endif /* ! __LINUX_REBOOT_H__ */
