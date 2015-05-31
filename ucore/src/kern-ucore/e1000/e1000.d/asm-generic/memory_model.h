#ifndef __ASM_GENERIC_MEMORY_MODEL_H__
#define __ASM_GENERIC_MEMORY_MODEL_H__

#define __pfn_to_page(pfn)	(vmemmap + (pfn))
#define __page_to_pfn(page)	(unsigned long)((page) - vmemmap)

#define page_to_pfn __page_to_pfn
#define pfn_to_page __pfn_to_page

#endif /* ! __ASM_GENERIC_MEMORY_MODEL_H__ */
