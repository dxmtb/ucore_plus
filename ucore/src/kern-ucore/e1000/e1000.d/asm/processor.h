#ifndef __ASM_PROCESSOR_H__
#define __ASM_PROCESSOR_H__

#include <asm/processor-flags.h>

struct task_struct;
struct mm_struct;

#include <asm/vm86.h>
#include <asm/math_emu.h>
#include <asm/segment.h>
#include <asm/types.h>
#include <asm/sigcontext.h>
#include <asm/current.h>
#include <asm/cpufeature.h>
#include <asm/page.h>
#include <asm/pgtable_types.h>
#include <asm/percpu.h>
#include <asm/msr.h>
#include <asm/desc_defs.h>
#include <asm/nops.h>
#include <asm/special_insns.h>

#include <linux/personality.h>
#include <linux/cpumask.h>
#include <linux/cache.h>
#include <linux/threads.h>
#include <linux/math64.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/irqflags.h>

/*
 * We handle most unaligned accesses in hardware.  On the other hand
 * unaligned DMA can be quite expensive on some Nehalem processors.
 *
 * Based on this we disable the IP header alignment in network drivers.
 */
#define NET_IP_ALIGN	0

#define HBP_NUM 4
/*
 * Default implementation of macro that returns current
 * instruction pointer ("program counter").
 */
static inline void *current_text_addr(void)
{
	void *pc;

	asm volatile("mov $1f, %0; 1:":"=r" (pc));

	return pc;
}

struct cpuinfo_x86 {
	__u8			x86;		/* CPU family */
	__u8			x86_vendor;	/* CPU vendor */
	__u8			x86_model;
	__u8			x86_mask;
#ifdef CONFIG_X86_32
	char			wp_works_ok;	/* It doesn't on 386's */

	/* Problems on some 486Dx4's and old 386's: */
	char			hard_math;
	char			rfu;
	char			pad0;
#else
	/* Number of 4K pages in DTLB/ITLB combined(in pages): */
	int			x86_tlbsize;
#endif
	__u8			x86_virt_bits;
	__u8			x86_phys_bits;
	/* CPUID returned core id bits: */
	__u8			x86_coreid_bits;
	/* Max extended CPUID function supported: */
	__u32			extended_cpuid_level;
	/* Maximum supported CPUID level, -1=no CPUID: */
	int			cpuid_level;
	__u32			x86_capability[NCAPINTS + NBUGINTS];
	char			x86_vendor_id[16];
	char			x86_model_id[64];
	/* in KB - valid for CPUS which support this call: */
	int			x86_cache_size;
	int			x86_cache_alignment;	/* In bytes */
	int			x86_power;
	unsigned long		loops_per_jiffy;
	/* cpuid returned max cores value: */
	u16			 x86_max_cores;
	u16			apicid;
	u16			initial_apicid;
	u16			x86_clflush_size;
	/* number of cores as seen by the OS: */
	u16			booted_cores;
	/* Physical processor id: */
	u16			phys_proc_id;
	/* Core id: */
	u16			cpu_core_id;
	/* Compute unit id */
	u8			compute_unit_id;
	/* Index into per_cpu list: */
	u16			cpu_index;
	u32			microcode;
} __attribute__((__aligned__(SMP_CACHE_BYTES)));

#define X86_VENDOR_INTEL	0

#define X86_VENDOR_AMD		2

#define X86_VENDOR_CENTAUR	5

/*
 * capabilities of CPUs
 */
extern struct cpuinfo_x86	boot_cpu_data;

extern struct tss_struct	doublefault_tss;

DECLARE_PER_CPU_SHARED_ALIGNED(struct cpuinfo_x86, cpu_info);
#define cpu_data(cpu)		per_cpu(cpu_info, cpu)

extern const struct seq_operations cpuinfo_op;

static inline void native_cpuid(unsigned int *eax, unsigned int *ebx,
				unsigned int *ecx, unsigned int *edx)
{
	/* ecx is often an input as well as an output. */
	asm volatile("cpuid"
	    : "=a" (*eax),
	      "=b" (*ebx),
	      "=c" (*ecx),
	      "=d" (*edx)
	    : "0" (*eax), "2" (*ecx)
	    : "memory");
}

struct x86_hw_tss {
	u32			reserved1;
	u64			sp0;
	u64			sp1;
	u64			sp2;
	u64			reserved2;
	u64			ist[7];
	u32			reserved3;
	u32			reserved4;
	u16			reserved5;
	u16			io_bitmap_base;

} __attribute__((packed)) ____cacheline_aligned;

/*
 * IO-bitmap sizes:
 */
#define IO_BITMAP_BITS			65536
#define IO_BITMAP_BYTES			(IO_BITMAP_BITS/8)
#define IO_BITMAP_LONGS			(IO_BITMAP_BYTES/sizeof(long))

struct tss_struct {
	/*
	 * The hardware state:
	 */
	struct x86_hw_tss	x86_tss;

	/*
	 * The extra 1 is there because the CPU will access an
	 * additional byte beyond the end of the IO permission
	 * bitmap. The extra byte must be all 1 bits, and must
	 * be within the limit.
	 */
	unsigned long		io_bitmap[IO_BITMAP_LONGS + 1];

	/*
	 * .. and then another 0x100 bytes for the emergency kernel stack:
	 */
	unsigned long		stack[64];

} ____cacheline_aligned;

struct i387_fsave_struct {
	u32			cwd;	/* FPU Control Word		*/
	u32			swd;	/* FPU Status Word		*/
	u32			twd;	/* FPU Tag Word			*/
	u32			fip;	/* FPU IP Offset		*/
	u32			fcs;	/* FPU IP Selector		*/
	u32			foo;	/* FPU Operand Pointer Offset	*/
	u32			fos;	/* FPU Operand Pointer Selector	*/

	/* 8*10 bytes for each FP-reg = 80 bytes:			*/
	u32			st_space[20];

	/* Software status information [not touched by FSAVE ]:		*/
	u32			status;
};

struct i387_fxsave_struct {
	u16			cwd; /* Control Word			*/
	u16			swd; /* Status Word			*/
	u16			twd; /* Tag Word			*/
	u16			fop; /* Last Instruction Opcode		*/
	union {
		struct {
			u64	rip; /* Instruction Pointer		*/
			u64	rdp; /* Data Pointer			*/
		};
		struct {
			u32	fip; /* FPU IP Offset			*/
			u32	fcs; /* FPU IP Selector			*/
			u32	foo; /* FPU Operand Offset		*/
			u32	fos; /* FPU Operand Selector		*/
		};
	};
	u32			mxcsr;		/* MXCSR Register State */
	u32			mxcsr_mask;	/* MXCSR Mask		*/

	/* 8*16 bytes for each FP-reg = 128 bytes:			*/
	u32			st_space[32];

	/* 16*16 bytes for each XMM-reg = 256 bytes:			*/
	u32			xmm_space[64];

	u32			padding[12];

	union {
		u32		padding1[12];
		u32		sw_reserved[12];
	};

} __attribute__((aligned(16)));

struct i387_soft_struct {
	u32			cwd;
	u32			swd;
	u32			twd;
	u32			fip;
	u32			fcs;
	u32			foo;
	u32			fos;
	/* 8*10 bytes for each FP-reg = 80 bytes: */
	u32			st_space[20];
	u8			ftop;
	u8			changed;
	u8			lookahead;
	u8			no_update;
	u8			rm;
	u8			alimit;
	struct math_emu_info	*info;
	u32			entry_eip;
};

struct ymmh_struct {
	/* 16 * 16 bytes for each YMMH-reg = 256 bytes */
	u32 ymmh_space[64];
};

struct xsave_hdr_struct {
	u64 xstate_bv;
	u64 reserved1[2];
	u64 reserved2[5];
} __attribute__((packed));

struct xsave_struct {
	struct i387_fxsave_struct i387;
	struct xsave_hdr_struct xsave_hdr;
	struct ymmh_struct ymmh;
	/* new processor state extensions will go here */
} __attribute__ ((packed, aligned (64)));

union thread_xstate {
	struct i387_fsave_struct	fsave;
	struct i387_fxsave_struct	fxsave;
	struct i387_soft_struct		soft;
	struct xsave_struct		xsave;
};

struct fpu {
	unsigned int last_cpu;
	unsigned int has_fpu;
	union thread_xstate *state;
};

union irq_stack_union {
	char irq_stack[IRQ_STACK_SIZE];
	/*
	 * GCC hardcodes the stack canary as %gs:40.  Since the
	 * irq_stack is the object at %gs:0, we reserve the bottom
	 * 48 bytes of the irq stack for the canary.
	 */
	struct {
		char gs_base[40];
		unsigned long stack_canary;
	};
};

DECLARE_PER_CPU_FIRST(union irq_stack_union, irq_stack_union);

extern struct kmem_cache *task_xstate_cachep;

struct perf_event;

struct thread_struct {
	/* Cached TLS descriptors: */
	struct desc_struct	tls_array[GDT_ENTRY_TLS_ENTRIES];
	unsigned long		sp0;
	unsigned long		sp;
#ifdef CONFIG_X86_32
	unsigned long		sysenter_cs;
#else
	unsigned long		usersp;	/* Copy from PDA */
	unsigned short		es;
	unsigned short		ds;
	unsigned short		fsindex;
	unsigned short		gsindex;
#endif
#ifdef CONFIG_X86_32
	unsigned long		ip;
#endif
#ifdef CONFIG_X86_64
	unsigned long		fs;
#endif
	unsigned long		gs;
	/* Save middle states of ptrace breakpoints */
	struct perf_event	*ptrace_bps[HBP_NUM];
	/* Debug status used for traps, single steps, etc... */
	unsigned long           debugreg6;
	/* Keep track of the exact dr7 value set by the user */
	unsigned long           ptrace_dr7;
	/* Fault info: */
	unsigned long		cr2;
	unsigned long		trap_nr;
	unsigned long		error_code;
	/* floating point and extended processor state */
	struct fpu		fpu;
#ifdef CONFIG_X86_32
	/* Virtual 86 mode info */
	struct vm86_struct __user *vm86_info;
	unsigned long		screen_bitmap;
	unsigned long		v86flags;
	unsigned long		v86mask;
	unsigned long		saved_sp0;
	unsigned int		saved_fs;
	unsigned int		saved_gs;
#endif
	/* IO permissions: */
	unsigned long		*io_bitmap_ptr;
	unsigned long		iopl;
	/* Max allowed port in the bitmap, in bytes: */
	unsigned		io_bitmap_max;
};

static inline void
native_load_sp0(struct tss_struct *tss, struct thread_struct *thread)
{
	tss->x86_tss.sp0 = thread->sp0;
#ifdef CONFIG_X86_32
	/* Only happens when SEP is enabled, no need to test "SEP"arately: */
	if (unlikely(tss->x86_tss.ss1 != thread->sysenter_cs)) {
		tss->x86_tss.ss1 = thread->sysenter_cs;
		wrmsr(MSR_IA32_SYSENTER_CS, thread->sysenter_cs, 0);
	}
#endif
}

#define __cpuid			native_cpuid

/*
 * Save the cr4 feature set we're using (ie
 * Pentium 4MB enable and PPro Global page
 * enable), so that any CPU's that boot up
 * after us can get the correct flags.
 */
extern unsigned long mmu_cr4_features;
extern u32 *trampoline_cr4_features;

typedef struct {
	unsigned long		seg;
} mm_segment_t;

/*
 * Generic CPUID function
 * clear %ecx since some cpus (Cyrix MII) do not set or clear %ecx
 * resulting in stale register contents being returned.
 */
static inline void cpuid(unsigned int op,
			 unsigned int *eax, unsigned int *ebx,
			 unsigned int *ecx, unsigned int *edx)
{
	*eax = op;
	*ecx = 0;
	__cpuid(eax, ebx, ecx, edx);
}

static inline void rep_nop(void)
{
	asm volatile("rep; nop" ::: "memory");
}

static inline void cpu_relax(void)
{
	rep_nop();
}

extern bool			amd_e400_c1e_detected;

#define ARCH_HAS_PREFETCHW
#define ARCH_HAS_SPINLOCK_PREFETCH

# define BASE_PREFETCH		"prefetcht0 (%1)"

/*
 * 3dnow prefetch to get an exclusive cache line.
 * Useful for spinlocks to avoid one state transition in the
 * cache coherency protocol:
 */
static inline void prefetchw(const void *x)
{
	alternative_input(BASE_PREFETCH,
			  "prefetchw (%1)",
			  X86_FEATURE_3DNOW,
			  "r" (x));
}

#define TASK_SIZE_OF(child)	((test_tsk_thread_flag(child, TIF_ADDR32)) ? \
					IA32_PAGE_OFFSET : TASK_SIZE_MAX)

#define task_pt_regs(tsk)	((struct pt_regs *)(tsk)->thread.sp0 - 1)

/*
 * User space RSP while inside the SYSCALL fast path
 */
DECLARE_PER_CPU(unsigned long, old_rsp);

#define APERFMPERF_SHIFT 10

#endif /* ! __ASM_PROCESSOR_H__ */
