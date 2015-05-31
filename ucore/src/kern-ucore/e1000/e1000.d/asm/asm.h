#ifndef __ASM_ASM_H__
#define __ASM_ASM_H__

# define __ASM_FORM(x)	" " #x " "
# define __ASM_FORM_COMMA(x) " " #x ","

# define __ASM_SEL(a,b) __ASM_FORM(b)

#define __ASM_SIZE(inst, ...)	__ASM_SEL(inst##l##__VA_ARGS__, \
					  inst##q##__VA_ARGS__)

#define _ASM_INC	__ASM_SIZE(inc)

#define _ASM_ADD	__ASM_SIZE(add)

# define _ASM_EXTABLE(from,to)					\
	" .pushsection \"__ex_table\",\"a\"\n"			\
	" .balign 8\n"						\
	" .long (" #from ") - .\n"				\
	" .long (" #to ") - .\n"				\
	" .popsection\n"

#endif /* ! __ASM_ASM_H__ */
