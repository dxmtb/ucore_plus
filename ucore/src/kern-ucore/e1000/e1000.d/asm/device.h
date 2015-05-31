#ifndef __ASM_DEVICE_H__
#define __ASM_DEVICE_H__

struct dev_archdata {
#ifdef CONFIG_X86_DEV_DMA_OPS
	struct dma_map_ops *dma_ops;
#endif
#if defined(CONFIG_INTEL_IOMMU) || defined(CONFIG_AMD_IOMMU)
	void *iommu; /* hook for IOMMU specific extension */
#endif
};

#endif /* ! __ASM_DEVICE_H__ */
