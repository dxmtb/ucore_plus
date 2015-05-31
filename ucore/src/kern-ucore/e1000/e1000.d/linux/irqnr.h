#ifndef __LINUX_IRQNR_H__
#define __LINUX_IRQNR_H__

#include <uapi/linux/irqnr.h>

extern struct irq_desc *irq_to_desc(unsigned int irq);

#endif /* ! __LINUX_IRQNR_H__ */
