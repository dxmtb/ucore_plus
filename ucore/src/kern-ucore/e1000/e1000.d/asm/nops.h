#ifndef __ASM_NOPS_H__
#define __ASM_NOPS_H__

/* generic versions from gas
   1: nop
   the following instructions are NOT nops in 64-bit mode,
   for 64-bit mode use K8 or P6 nops instead
   2: movl %esi,%esi
   3: leal 0x00(%esi),%esi
   4: leal 0x00(,%esi,1),%esi
   6: leal 0x00000000(%esi),%esi
   7: leal 0x00000000(,%esi,1),%esi
*/
#define GENERIC_NOP1 0x90

/* Opteron 64bit nops
   1: nop
   2: osp nop
   3: osp osp nop
   4: osp osp osp nop
*/
#define K8_NOP1 GENERIC_NOP1
#define K8_NOP2	0x66,K8_NOP1
#define K8_NOP3	0x66,K8_NOP2
#define K8_NOP4	0x66,K8_NOP3
#define K8_NOP5	K8_NOP3,K8_NOP2

#define _ASM_MK_NOP(x) ".byte " __stringify(x) "\n"

#define ASM_NOP3 _ASM_MK_NOP(K8_NOP3)
#define ASM_NOP4 _ASM_MK_NOP(K8_NOP4)
#define ASM_NOP5 _ASM_MK_NOP(K8_NOP5)

#endif /* ! __ASM_NOPS_H__ */
