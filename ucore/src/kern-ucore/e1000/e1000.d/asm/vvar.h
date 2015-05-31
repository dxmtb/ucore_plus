#ifndef __ASM_VVAR_H__
#define __ASM_VVAR_H__

#define VVAR_ADDRESS (-10*1024*1024 - 4096)

#define DECLARE_VVAR(offset, type, name)				\
	static type const * const vvaraddr_ ## name =			\
		(void *)(VVAR_ADDRESS + (offset));

#define VVAR(name) (*vvaraddr_ ## name)

DECLARE_VVAR(16, int, vgetcpu_mode)

#undef DECLARE_VVAR

#endif /* ! __ASM_VVAR_H__ */
