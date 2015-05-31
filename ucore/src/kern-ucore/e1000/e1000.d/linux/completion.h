#ifndef __LINUX_COMPLETION_H__
#define __LINUX_COMPLETION_H__

#include <linux/wait.h>

/*
 * struct completion - structure used to maintain state for a "completion"
 *
 * This is the opaque structure used to maintain the state for a "completion".
 * Completions currently use a FIFO to queue threads that have to wait for
 * the "completion" event.
 *
 * See also:  complete(), wait_for_completion() (and friends _timeout,
 * _interruptible, _interruptible_timeout, and _killable), init_completion(),
 * and macros DECLARE_COMPLETION(), DECLARE_COMPLETION_ONSTACK(), and
 * INIT_COMPLETION().
 */
struct completion {
	unsigned int done;
	wait_queue_head_t wait;
};

#endif /* ! __LINUX_COMPLETION_H__ */
