#include <sem.h>
#include <spinlock.h>
#include <clock.h>
#include <lwip/err.h>
#include <lwip/sys.h>
#include "sys_arch.h"
#include "cc.h"

void sys_init(void) {}

int millitime(void) { return ticks * 10; }

/*
err_t sys_sem_new(sys_sem_t *sem, u8_t count) {
    return ERR_OK;
}

void sys_sem_free(sys_sem_t *sem) {
}

void sys_sem_signal(sys_sem_t *sem) {
}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout) {
    return 0;
}

int sys_sem_valid(sys_sem_t *sem) {
}

void sys_sem_set_invalid(sys_sem_t *sem) {
}
*/

err_t sys_sem_new(sys_sem_t *sem_p, u8_t count) {
    semaphore_t *sem = kmalloc(sizeof(semaphore_t));
    if (sem == NULL) {
        return ERR_MEM;
    }
    sem_init(sem, count);
    *sem_p = sem;
    return ERR_OK;
}

void sys_sem_free(sys_sem_t *sem) {
    kfree(*sem);
    *sem = NULL;
}

void sys_sem_signal(sys_sem_t *sem) {
    assert(sem != NULL && *sem != NULL);
    up(*sem);
}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout) {
//timeout ignored
    if (timeout == 0) {
        down(*sem);
        return 0;
    }
    int now = millitime();
    while(millitime() - now < timeout) {
        bool ret = try_down(*sem);
        if (ret == 1)
            return millitime() - now;
    }
    return SYS_ARCH_TIMEOUT;
}

int sys_sem_valid(sys_sem_t *sem) {
    return sem != NULL && *sem != 0;
}

void sys_sem_set_invalid(sys_sem_t *sem) {
    *sem = 0;
}

err_t sys_mbox_new(sys_mbox_t *mbox_p, int size)
{
    *mbox_p = kmalloc(sizeof(struct mbox));
    struct mbox *mbox = *mbox_p;
    spinlock_init(&(mbox->lock));
    sys_sem_new(&mbox->free, NSLOTS);
    sys_sem_new(&mbox->queued, 0);
    mbox->count = 0;
    mbox->head = -1;
    mbox->next = 0;
    return ERR_OK;
};

void sys_mbox_free(sys_mbox_t *mbox)
{
    kfree(*mbox);
}

void sys_mbox_post(sys_mbox_t *mbox_p, void *msg)
{
    struct mbox *mbox = *mbox_p;
	unsigned long flags;
    sys_arch_sem_wait(&mbox->free, 0);
    spin_lock_irqsave(&mbox->lock, flags);
    if (mbox->count == NSLOTS)
    {
        spin_unlock_irqrestore(&mbox->lock, flags);
        return;
    }
    int slot = mbox->next;
    mbox->next = (slot + 1) % NSLOTS;
    mbox->slots[slot] = msg;
    mbox->count++;
    if (mbox->head == -1)
        mbox->head = slot;

    sys_sem_signal(&mbox->queued);
    spin_unlock_irqrestore(&mbox->lock, flags);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox_p, void *msg) {
    struct mbox *mbox = *mbox_p;
	unsigned long flags;
    if (sys_arch_sem_wait(&mbox->free, 1) == SYS_ARCH_TIMEOUT) {
        return ERR_MEM;
    }
    spin_lock_irqsave(&mbox->lock, flags);
    if (mbox->count == NSLOTS)
    {
        spin_unlock_irqrestore(&mbox->lock, flags);
        return ERR_MEM;
    }
    int slot = mbox->next;
    mbox->next = (slot + 1) % NSLOTS;
    mbox->slots[slot] = msg;
    mbox->count++;
    if (mbox->head == -1)
        mbox->head = slot;

    sys_sem_signal(&mbox->queued);
    spin_unlock_irqrestore(&mbox->lock, flags);
    return ERR_OK;
}

u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox_p, void **msg, u32_t timeout)
{
    struct mbox *mbox = *mbox_p;
    assert(mbox);
	unsigned long flags;
    u32_t waited = sys_arch_sem_wait(&mbox->queued, timeout);
    spin_lock_irqsave(&mbox->lock, flags);
    if (waited == SYS_ARCH_TIMEOUT)
    {
        spin_unlock_irqrestore(&mbox->lock, flags);
        return waited;
    }

    int slot = mbox->head;
    if (slot == -1)
    {
        spin_unlock_irqrestore(&mbox->lock, flags);
        return SYS_ARCH_TIMEOUT; // XXX panic is not good...
    }

    if (msg)
        *msg = mbox->slots[slot];

    mbox->head = (slot + 1) % NSLOTS;
    mbox->count--;
    if (mbox->count == 0)
        mbox->head = -1;

    sys_sem_signal(&mbox->free);
    spin_unlock_irqrestore(&mbox->lock, flags);
    return waited;
}

int sys_mbox_valid(sys_mbox_t *mbox) {
    return mbox != NULL && *mbox != NULL;
}

void sys_mbox_set_invalid(sys_mbox_t *mbox) {
    *mbox = NULL;
}

u32_t sys_now(void)
{
    return millitime();
}

sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
    int pid = kernel_thread(thread, arg, 0);
	set_proc_name(find_proc(pid), name);
    return pid;
}
