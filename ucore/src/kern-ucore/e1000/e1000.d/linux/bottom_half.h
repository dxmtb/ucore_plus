#ifndef __LINUX_BOTTOM_HALF_H__
#define __LINUX_BOTTOM_HALF_H__

extern void local_bh_disable(void);

extern void local_bh_enable(void);
extern void local_bh_enable_ip(unsigned long ip);

#endif /* ! __LINUX_BOTTOM_HALF_H__ */
