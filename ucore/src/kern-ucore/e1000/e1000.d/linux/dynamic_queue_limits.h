#ifndef __LINUX_DYNAMIC_QUEUE_LIMITS_H__
#define __LINUX_DYNAMIC_QUEUE_LIMITS_H__

struct dql {
	/* Fields accessed in enqueue path (dql_queued) */
	unsigned int	num_queued;		/* Total ever queued */
	unsigned int	adj_limit;		/* limit + num_completed */
	unsigned int	last_obj_cnt;		/* Count at last queuing */

	/* Fields accessed only by completion path (dql_completed) */

	unsigned int	limit ____cacheline_aligned_in_smp; /* Current limit */
	unsigned int	num_completed;		/* Total ever completed */

	unsigned int	prev_ovlimit;		/* Previous over limit */
	unsigned int	prev_num_queued;	/* Previous queue total */
	unsigned int	prev_last_obj_cnt;	/* Previous queuing cnt */

	unsigned int	lowest_slack;		/* Lowest slack found */
	unsigned long	slack_start_time;	/* Time slacks seen */

	/* Configuration */
	unsigned int	max_limit;		/* Max limit */
	unsigned int	min_limit;		/* Minimum limit */
	unsigned int	slack_hold_time;	/* Time to measure slack */
};

#define DQL_MAX_OBJECT (UINT_MAX / 16)

/*
 * Record number of objects queued. Assumes that caller has already checked
 * availability in the queue with dql_avail.
 */
static inline void dql_queued(struct dql *dql, unsigned int count)
{
	BUG_ON(count > DQL_MAX_OBJECT);

	dql->num_queued += count;
	dql->last_obj_cnt = count;
}

static inline int dql_avail(const struct dql *dql)
{
	return dql->adj_limit - dql->num_queued;
}

void dql_completed(struct dql *dql, unsigned int count);

void dql_reset(struct dql *dql);

#endif /* ! __LINUX_DYNAMIC_QUEUE_LIMITS_H__ */
