#ifndef __ASM_GENERIC_DMA_COHERENT_H__
#define __ASM_GENERIC_DMA_COHERENT_H__

#define dma_alloc_from_coherent(dev, size, handle, ret) (0)
#define dma_release_from_coherent(dev, order, vaddr) (0)

#endif /* ! __ASM_GENERIC_DMA_COHERENT_H__ */
