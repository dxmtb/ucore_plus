#ifndef __LINUX_DMA_DEBUG_H__
#define __LINUX_DMA_DEBUG_H__

#include <linux/types.h>

struct device;
struct scatterlist;
struct bus_type;

static inline void debug_dma_map_page(struct device *dev, struct page *page,
				      size_t offset, size_t size,
				      int direction, dma_addr_t dma_addr,
				      bool map_single)
{
}

static inline void debug_dma_mapping_error(struct device *dev,
					  dma_addr_t dma_addr)
{
}

static inline void debug_dma_unmap_page(struct device *dev, dma_addr_t addr,
					size_t size, int direction,
					bool map_single)
{
}

static inline void debug_dma_map_sg(struct device *dev, struct scatterlist *sg,
				    int nents, int mapped_ents, int direction)
{
}

static inline void debug_dma_unmap_sg(struct device *dev,
				      struct scatterlist *sglist,
				      int nelems, int dir)
{
}

static inline void debug_dma_alloc_coherent(struct device *dev, size_t size,
					    dma_addr_t dma_addr, void *virt)
{
}

static inline void debug_dma_free_coherent(struct device *dev, size_t size,
					   void *virt, dma_addr_t addr)
{
}

static inline void debug_dma_sync_single_for_cpu(struct device *dev,
						 dma_addr_t dma_handle,
						 size_t size, int direction)
{
}

static inline void debug_dma_sync_single_for_device(struct device *dev,
						    dma_addr_t dma_handle,
						    size_t size, int direction)
{
}

static inline void debug_dma_sync_single_range_for_cpu(struct device *dev,
						       dma_addr_t dma_handle,
						       unsigned long offset,
						       size_t size,
						       int direction)
{
}

static inline void debug_dma_sync_single_range_for_device(struct device *dev,
							  dma_addr_t dma_handle,
							  unsigned long offset,
							  size_t size,
							  int direction)
{
}

static inline void debug_dma_sync_sg_for_cpu(struct device *dev,
					     struct scatterlist *sg,
					     int nelems, int direction)
{
}

static inline void debug_dma_sync_sg_for_device(struct device *dev,
						struct scatterlist *sg,
						int nelems, int direction)
{
}

#endif /* ! __LINUX_DMA_DEBUG_H__ */
