#ifndef __LINUX_DMAENGINE_H__
#define __LINUX_DMAENGINE_H__

#include <linux/device.h>
#include <linux/uio.h>
#include <linux/bug.h>
#include <linux/scatterlist.h>
#include <linux/bitmap.h>
#include <linux/types.h>
#include <asm/page.h>

/**
 * typedef dma_cookie_t - an opaque DMA cookie
 *
 * if dma_cookie_t is >0 it's a DMA request cookie, <0 it's an error code
 */
typedef s32 dma_cookie_t;

/**
 * enum dma_status - DMA transaction status
 * @DMA_SUCCESS: transaction completed successfully
 * @DMA_IN_PROGRESS: transaction not yet processed
 * @DMA_PAUSED: transaction is paused
 * @DMA_ERROR: transaction failed
 */
enum dma_status {
	DMA_SUCCESS,
	DMA_IN_PROGRESS,
	DMA_PAUSED,
	DMA_ERROR,
};

/**
 * enum dma_transaction_type - DMA transaction types/indexes
 *
 * Note: The DMA_ASYNC_TX capability is not to be set by drivers.  It is
 * automatically set as dma devices are registered.
 */
enum dma_transaction_type {
	DMA_MEMCPY,
	DMA_XOR,
	DMA_PQ,
	DMA_XOR_VAL,
	DMA_PQ_VAL,
	DMA_MEMSET,
	DMA_INTERRUPT,
	DMA_SG,
	DMA_PRIVATE,
	DMA_ASYNC_TX,
	DMA_SLAVE,
	DMA_CYCLIC,
	DMA_INTERLEAVE,
/* last transaction type for creation of the capabilities mask */
	DMA_TX_TYPE_END,
};

/**
 * enum dma_transfer_direction - dma transfer mode and direction indicator
 * @DMA_MEM_TO_MEM: Async/Memcpy mode
 * @DMA_MEM_TO_DEV: Slave mode & From Memory to Device
 * @DMA_DEV_TO_MEM: Slave mode & From Device to Memory
 * @DMA_DEV_TO_DEV: Slave mode & From Device to Device
 */
enum dma_transfer_direction {
	DMA_MEM_TO_MEM,
	DMA_MEM_TO_DEV,
	DMA_DEV_TO_MEM,
	DMA_DEV_TO_DEV,
	DMA_TRANS_NONE,
};

/**
 * struct data_chunk - Element of scatter-gather list that makes a frame.
 * @size: Number of bytes to read from source.
 *	  size_dst := fn(op, size_src), so doesn't mean much for destination.
 * @icg: Number of bytes to jump after last src/dst address of this
 *	 chunk and before first src/dst address for next chunk.
 *	 Ignored for dst(assumed 0), if dst_inc is true and dst_sgl is false.
 *	 Ignored for src(assumed 0), if src_inc is true and src_sgl is false.
 */
struct data_chunk {
	size_t size;
	size_t icg;
};

/**
 * struct dma_interleaved_template - Template to convey DMAC the transfer pattern
 *	 and attributes.
 * @src_start: Bus address of source for the first chunk.
 * @dst_start: Bus address of destination for the first chunk.
 * @dir: Specifies the type of Source and Destination.
 * @src_inc: If the source address increments after reading from it.
 * @dst_inc: If the destination address increments after writing to it.
 * @src_sgl: If the 'icg' of sgl[] applies to Source (scattered read).
 *		Otherwise, source is read contiguously (icg ignored).
 *		Ignored if src_inc is false.
 * @dst_sgl: If the 'icg' of sgl[] applies to Destination (scattered write).
 *		Otherwise, destination is filled contiguously (icg ignored).
 *		Ignored if dst_inc is false.
 * @numf: Number of frames in this template.
 * @frame_size: Number of chunks in a frame i.e, size of sgl[].
 * @sgl: Array of {chunk,icg} pairs that make up a frame.
 */
struct dma_interleaved_template {
	dma_addr_t src_start;
	dma_addr_t dst_start;
	enum dma_transfer_direction dir;
	bool src_inc;
	bool dst_inc;
	bool src_sgl;
	bool dst_sgl;
	size_t numf;
	size_t frame_size;
	struct data_chunk sgl[0];
};

/**
 * enum dma_ctrl_flags - DMA flags to augment operation preparation,
 *  control completion, and communicate status.
 * @DMA_PREP_INTERRUPT - trigger an interrupt (callback) upon completion of
 *  this transaction
 * @DMA_CTRL_ACK - if clear, the descriptor cannot be reused until the client
 *  acknowledges receipt, i.e. has has a chance to establish any dependency
 *  chains
 * @DMA_COMPL_SKIP_SRC_UNMAP - set to disable dma-unmapping the source buffer(s)
 * @DMA_COMPL_SKIP_DEST_UNMAP - set to disable dma-unmapping the destination(s)
 * @DMA_COMPL_SRC_UNMAP_SINGLE - set to do the source dma-unmapping as single
 * 	(if not set, do the source dma-unmapping as page)
 * @DMA_COMPL_DEST_UNMAP_SINGLE - set to do the destination dma-unmapping as single
 * 	(if not set, do the destination dma-unmapping as page)
 * @DMA_PREP_PQ_DISABLE_P - prevent generation of P while generating Q
 * @DMA_PREP_PQ_DISABLE_Q - prevent generation of Q while generating P
 * @DMA_PREP_CONTINUE - indicate to a driver that it is reusing buffers as
 *  sources that were the result of a previous operation, in the case of a PQ
 *  operation it continues the calculation with new sources
 * @DMA_PREP_FENCE - tell the driver that subsequent operations depend
 *  on the result of this operation
 */
enum dma_ctrl_flags {
	DMA_PREP_INTERRUPT = (1 << 0),
	DMA_CTRL_ACK = (1 << 1),
	DMA_COMPL_SKIP_SRC_UNMAP = (1 << 2),
	DMA_COMPL_SKIP_DEST_UNMAP = (1 << 3),
	DMA_COMPL_SRC_UNMAP_SINGLE = (1 << 4),
	DMA_COMPL_DEST_UNMAP_SINGLE = (1 << 5),
	DMA_PREP_PQ_DISABLE_P = (1 << 6),
	DMA_PREP_PQ_DISABLE_Q = (1 << 7),
	DMA_PREP_CONTINUE = (1 << 8),
	DMA_PREP_FENCE = (1 << 9),
};

/**
 * enum dma_ctrl_cmd - DMA operations that can optionally be exercised
 * on a running channel.
 * @DMA_TERMINATE_ALL: terminate all ongoing transfers
 * @DMA_PAUSE: pause ongoing transfers
 * @DMA_RESUME: resume paused transfer
 * @DMA_SLAVE_CONFIG: this command is only implemented by DMA controllers
 * that need to runtime reconfigure the slave channels (as opposed to passing
 * configuration data in statically from the platform). An additional
 * argument of struct dma_slave_config must be passed in with this
 * command.
 * @FSLDMA_EXTERNAL_START: this command will put the Freescale DMA controller
 * into external start mode.
 */
enum dma_ctrl_cmd {
	DMA_TERMINATE_ALL,
	DMA_PAUSE,
	DMA_RESUME,
	DMA_SLAVE_CONFIG,
	FSLDMA_EXTERNAL_START,
};

/**
 * enum sum_check_bits - bit position of pq_check_flags
 */
enum sum_check_bits {
	SUM_CHECK_P = 0,
	SUM_CHECK_Q = 1,
};

/**
 * enum pq_check_flags - result of async_{xor,pq}_zero_sum operations
 * @SUM_CHECK_P_RESULT - 1 if xor zero sum error, 0 otherwise
 * @SUM_CHECK_Q_RESULT - 1 if reed-solomon zero sum error, 0 otherwise
 */
enum sum_check_flags {
	SUM_CHECK_P_RESULT = (1 << SUM_CHECK_P),
	SUM_CHECK_Q_RESULT = (1 << SUM_CHECK_Q),
};

/**
 * dma_cap_mask_t - capabilities bitmap modeled after cpumask_t.
 * See linux/cpumask.h
 */
typedef struct { DECLARE_BITMAP(bits, DMA_TX_TYPE_END); } dma_cap_mask_t;

struct dma_chan_percpu {
	/* stats */
	unsigned long memcpy_count;
	unsigned long bytes_transferred;
};

/**
 * struct dma_chan - devices supply DMA channels, clients use them
 * @device: ptr to the dma device who supplies this channel, always !%NULL
 * @cookie: last cookie value returned to client
 * @completed_cookie: last completed cookie for this channel
 * @chan_id: channel ID for sysfs
 * @dev: class device for sysfs
 * @device_node: used to add this to the device chan list
 * @local: per-cpu pointer to a struct dma_chan_percpu
 * @client-count: how many clients are using this channel
 * @table_count: number of appearances in the mem-to-mem allocation table
 * @private: private data for certain client-channel associations
 */
struct dma_chan {
	struct dma_device *device;
	dma_cookie_t cookie;
	dma_cookie_t completed_cookie;

	/* sysfs */
	int chan_id;
	struct dma_chan_dev *dev;

	struct list_head device_node;
	struct dma_chan_percpu __percpu *local;
	int client_count;
	int table_count;
	void *private;
};

/**
 * struct dma_chan_dev - relate sysfs device node to backing channel device
 * @chan - driver channel device
 * @device - sysfs device
 * @dev_id - parent dma_device dev_id
 * @idr_ref - reference count to gate release of dma_device dev_id
 */
struct dma_chan_dev {
	struct dma_chan *chan;
	struct device device;
	int dev_id;
	atomic_t *idr_ref;
};

/**
 * typedef dma_filter_fn - callback filter for dma_request_channel
 * @chan: channel to be reviewed
 * @filter_param: opaque parameter passed through dma_request_channel
 *
 * When this optional parameter is specified in a call to dma_request_channel a
 * suitable channel is passed to this routine for further dispositioning before
 * being returned.  Where 'suitable' indicates a non-busy channel that
 * satisfies the given capability mask.  It returns 'true' to indicate that the
 * channel is suitable.
 */
typedef bool (*dma_filter_fn)(struct dma_chan *chan, void *filter_param);

typedef void (*dma_async_tx_callback)(void *dma_async_param);
/**
 * struct dma_async_tx_descriptor - async transaction descriptor
 * ---dma generic offload fields---
 * @cookie: tracking cookie for this transaction, set to -EBUSY if
 *	this tx is sitting on a dependency list
 * @flags: flags to augment operation preparation, control completion, and
 * 	communicate status
 * @phys: physical address of the descriptor
 * @chan: target channel for this operation
 * @tx_submit: set the prepared descriptor(s) to be executed by the engine
 * @callback: routine to call after this operation is complete
 * @callback_param: general parameter to pass to the callback routine
 * ---async_tx api specific fields---
 * @next: at completion submit this descriptor
 * @parent: pointer to the next level up in the dependency chain
 * @lock: protect the parent and next pointers
 */
struct dma_async_tx_descriptor {
	dma_cookie_t cookie;
	enum dma_ctrl_flags flags; /* not a 'long' to pack with cookie */
	dma_addr_t phys;
	struct dma_chan *chan;
	dma_cookie_t (*tx_submit)(struct dma_async_tx_descriptor *tx);
	dma_async_tx_callback callback;
	void *callback_param;
#ifdef CONFIG_ASYNC_TX_ENABLE_CHANNEL_SWITCH
	struct dma_async_tx_descriptor *next;
	struct dma_async_tx_descriptor *parent;
	spinlock_t lock;
#endif
};

/**
 * struct dma_tx_state - filled in to report the status of
 * a transfer.
 * @last: last completed DMA cookie
 * @used: last issued DMA cookie (i.e. the one in progress)
 * @residue: the remaining number of bytes left to transmit
 *	on the selected transfer for states DMA_IN_PROGRESS and
 *	DMA_PAUSED if this is implemented in the driver, else 0
 */
struct dma_tx_state {
	dma_cookie_t last;
	dma_cookie_t used;
	u32 residue;
};

/**
 * struct dma_device - info on the entity supplying DMA services
 * @chancnt: how many DMA channels are supported
 * @privatecnt: how many DMA channels are requested by dma_request_channel
 * @channels: the list of struct dma_chan
 * @global_node: list_head for global dma_device_list
 * @cap_mask: one or more dma_capability flags
 * @max_xor: maximum number of xor sources, 0 if no capability
 * @max_pq: maximum number of PQ sources and PQ-continue capability
 * @copy_align: alignment shift for memcpy operations
 * @xor_align: alignment shift for xor operations
 * @pq_align: alignment shift for pq operations
 * @fill_align: alignment shift for memset operations
 * @dev_id: unique device ID
 * @dev: struct device reference for dma mapping api
 * @device_alloc_chan_resources: allocate resources and return the
 *	number of allocated descriptors
 * @device_free_chan_resources: release DMA channel's resources
 * @device_prep_dma_memcpy: prepares a memcpy operation
 * @device_prep_dma_xor: prepares a xor operation
 * @device_prep_dma_xor_val: prepares a xor validation operation
 * @device_prep_dma_pq: prepares a pq operation
 * @device_prep_dma_pq_val: prepares a pqzero_sum operation
 * @device_prep_dma_memset: prepares a memset operation
 * @device_prep_dma_interrupt: prepares an end of chain interrupt operation
 * @device_prep_slave_sg: prepares a slave dma operation
 * @device_prep_dma_cyclic: prepare a cyclic dma operation suitable for audio.
 *	The function takes a buffer of size buf_len. The callback function will
 *	be called after period_len bytes have been transferred.
 * @device_prep_interleaved_dma: Transfer expression in a generic way.
 * @device_control: manipulate all pending operations on a channel, returns
 *	zero or error code
 * @device_tx_status: poll for transaction completion, the optional
 *	txstate parameter can be supplied with a pointer to get a
 *	struct with auxiliary transfer status information, otherwise the call
 *	will just return a simple status code
 * @device_issue_pending: push pending transactions to hardware
 */
struct dma_device {

	unsigned int chancnt;
	unsigned int privatecnt;
	struct list_head channels;
	struct list_head global_node;
	dma_cap_mask_t  cap_mask;
	unsigned short max_xor;
	unsigned short max_pq;
	u8 copy_align;
	u8 xor_align;
	u8 pq_align;
	u8 fill_align;
	#define DMA_HAS_PQ_CONTINUE (1 << 15)

	int dev_id;
	struct device *dev;

	int (*device_alloc_chan_resources)(struct dma_chan *chan);
	void (*device_free_chan_resources)(struct dma_chan *chan);

	struct dma_async_tx_descriptor *(*device_prep_dma_memcpy)(
		struct dma_chan *chan, dma_addr_t dest, dma_addr_t src,
		size_t len, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_xor)(
		struct dma_chan *chan, dma_addr_t dest, dma_addr_t *src,
		unsigned int src_cnt, size_t len, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_xor_val)(
		struct dma_chan *chan, dma_addr_t *src,	unsigned int src_cnt,
		size_t len, enum sum_check_flags *result, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_pq)(
		struct dma_chan *chan, dma_addr_t *dst, dma_addr_t *src,
		unsigned int src_cnt, const unsigned char *scf,
		size_t len, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_pq_val)(
		struct dma_chan *chan, dma_addr_t *pq, dma_addr_t *src,
		unsigned int src_cnt, const unsigned char *scf, size_t len,
		enum sum_check_flags *pqres, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_memset)(
		struct dma_chan *chan, dma_addr_t dest, int value, size_t len,
		unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_interrupt)(
		struct dma_chan *chan, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_dma_sg)(
		struct dma_chan *chan,
		struct scatterlist *dst_sg, unsigned int dst_nents,
		struct scatterlist *src_sg, unsigned int src_nents,
		unsigned long flags);

	struct dma_async_tx_descriptor *(*device_prep_slave_sg)(
		struct dma_chan *chan, struct scatterlist *sgl,
		unsigned int sg_len, enum dma_transfer_direction direction,
		unsigned long flags, void *context);
	struct dma_async_tx_descriptor *(*device_prep_dma_cyclic)(
		struct dma_chan *chan, dma_addr_t buf_addr, size_t buf_len,
		size_t period_len, enum dma_transfer_direction direction,
		unsigned long flags, void *context);
	struct dma_async_tx_descriptor *(*device_prep_interleaved_dma)(
		struct dma_chan *chan, struct dma_interleaved_template *xt,
		unsigned long flags);
	int (*device_control)(struct dma_chan *chan, enum dma_ctrl_cmd cmd,
		unsigned long arg);

	enum dma_status (*device_tx_status)(struct dma_chan *chan,
					    dma_cookie_t cookie,
					    struct dma_tx_state *txstate);
	void (*device_issue_pending)(struct dma_chan *chan);
};

static inline int dmaengine_device_control(struct dma_chan *chan,
					   enum dma_ctrl_cmd cmd,
					   unsigned long arg)
{
	if (chan->device->device_control)
		return chan->device->device_control(chan, cmd, arg);

	return -ENOSYS;
}

static inline bool dmaengine_check_align(u8 align, size_t off1, size_t off2, size_t len)
{
	size_t mask;

	if (!align)
		return true;
	mask = (1 << align) - 1;
	if (mask & (off1 | off2 | len))
		return false;
	return true;
}

static inline bool dmaf_continue(enum dma_ctrl_flags flags)
{
	return (flags & DMA_PREP_CONTINUE) == DMA_PREP_CONTINUE;
}

static inline bool dmaf_p_disabled_continue(enum dma_ctrl_flags flags)
{
	enum dma_ctrl_flags mask = DMA_PREP_CONTINUE | DMA_PREP_PQ_DISABLE_P;

	return (flags & mask) == mask;
}

static inline bool dma_dev_has_pq_continue(struct dma_device *dma)
{
	return (dma->max_pq & DMA_HAS_PQ_CONTINUE) == DMA_HAS_PQ_CONTINUE;
}

static inline unsigned short dma_dev_to_maxpq(struct dma_device *dma)
{
	return dma->max_pq & ~DMA_HAS_PQ_CONTINUE;
}

static inline struct dma_chan *__dma_request_channel(const dma_cap_mask_t *mask,
					      dma_filter_fn fn, void *fn_param)
{
	return NULL;
}
static inline struct dma_chan *dma_request_slave_channel(struct device *dev,
							 const char *name)
{
	return NULL;
}

#endif /* ! __LINUX_DMAENGINE_H__ */
