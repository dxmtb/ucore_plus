#ifndef LWIP_ARCH_SYS_ARCH_H
#define LWIP_ARCH_SYS_ARCH_H

#include <spinlock.h>
#include <sem.h>

typedef	semaphore_t *sys_sem_t;
typedef	sys_sem_t sys_mutex_t;

#define NSLOTS 128
struct mbox {
    struct spinlock_s lock;
    sys_sem_t free, queued;
    int count, head, next;
    void *slots[NSLOTS];
};
typedef struct mbox *sys_mbox_t;
typedef int sys_thread_t;

#define SYS_MBOX_NULL 0
#define SYS_SEM_NULL 0

#define LWIP_COMPAT_MUTEX 1

#define sys_arch_mbox_tryfetch(mbox,msg) \
      sys_arch_mbox_fetch(mbox,msg,1)

#endif
