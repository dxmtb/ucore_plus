#ifndef __UAPI_LINUX_PKT_CLS_H__
#define __UAPI_LINUX_PKT_CLS_H__

#include <linux/types.h>
#include <linux/pkt_sched.h>

/* I think i could have done better macros ; for now this is stolen from
 * some arch/mips code - jhs
*/
#define _TC_MAKE32(x) ((x))

#define _TC_MAKEMASK1(n) (_TC_MAKE32(1) << _TC_MAKE32(n))
#define _TC_MAKEMASK(v,n) (_TC_MAKE32((_TC_MAKE32(1)<<(v))-1) << _TC_MAKE32(n))
#define _TC_MAKEVALUE(v,n) (_TC_MAKE32(v) << _TC_MAKE32(n))

#define TC_MUNGED          _TC_MAKEMASK1(0)

#define CLR_TC_MUNGED(v)   ( v & ~TC_MUNGED)

#define TC_OK2MUNGE        _TC_MAKEMASK1(1)

#define CLR_TC_OK2MUNGE(v)   ( v & ~TC_OK2MUNGE)

#define S_TC_VERD          _TC_MAKE32(2)
#define M_TC_VERD          _TC_MAKEMASK(4,S_TC_VERD)

#define V_TC_VERD(x)       _TC_MAKEVALUE(x,S_TC_VERD)
#define SET_TC_VERD(v,n)   ((V_TC_VERD(n)) | (v & ~M_TC_VERD))

#endif /* ! __UAPI_LINUX_PKT_CLS_H__ */
