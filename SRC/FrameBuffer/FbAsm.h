#ifndef __FBASM_H_
#define __FBASM_H_


#include <stdio.h>
#include <linux/types.h>
#include <string.h>

#if defined(__i386__)

static inline void *fb_arch_memmove(void *d, const void *s, size_t count)
{
	int d0, d1, d2, d3;
	
	if (d < s) {
		__asm__ __volatile__ (
			"cld\n\t"
			"shrl $1,%%ecx\n\t"
			"jnc 1f\n\t"
			"movsb\n"
			"1:\tshrl $1,%%ecx\n\t"
			"jnc 2f\n\t"
			"movsw\n"
			"2:\trep\n\t"
			"movsl"
			: "=&c" (d0), "=&D" (d1), "=&S" (d2)
			:"0"(count),"1"((long)d),"2"((long)s)
			:"memory"
		);
	} else {
		__asm__ __volatile__ (
			"std\n\t"
			"shrl $1,%%ecx\n\t"
			"jnc 1f\n\t"
			"movb 3(%%esi),%%al\n\t"
			"movb %%al,3(%%edi)\n\t"
			"decl %%esi\n\t"
			"decl %%edi\n"
			"1:\tshrl $1,%%ecx\n\t"
			"jnc 2f\n\t"
			"movw 2(%%esi),%%ax\n\t"
			"movw %%ax,2(%%edi)\n\t"
			"decl %%esi\n\t"
			"decl %%edi\n\t"
			"decl %%esi\n\t"
			"decl %%edi\n"
			"2:\trep\n\t"
			"movsl\n\t"
			"cld"
			: "=&c" (d0), "=&D" (d1), "=&S" (d2), "=&a" (d3)
			:"0"(count),"1"(count-4+(long)d),"2"(count-4+(long)s)
			:"memory"
		);
	}

	return d;
}
#else
static inline void *fb_arch_memmove(void *d, const void *s, size_t count)
{
	return memmove(d, s, count);
}
#endif /* __i386__ */


//------------------------------------------------------------------------------
#if defined(__mips__)
static inline void *fb_arch_memset(void *s, int c, size_t n)
{
	__asm__ __volatile__ (
		"la\t$8,%0\n\t"
		"lb\t$9,%1\n\t"
		"lb\t$10,%2\n\t"

		"loop:\n\t"
		"sb\t$9,0($8)\n\t"
		"add\t$8,$8,1\n\t"
		"sub\t$10,$10,1\n\t"
		"beqz\t$10,exit\n\t"
		"j\tloop\n\t"

		"exit:\n\t"
		:
		: "g" (s), "g" (c), "g" (n)
                : "$8", "$9", "$10", "memory"
	);
}
#elif defined(__i386__)
static inline void *fb_arch_memset(void *s, int c, size_t n)
{
	int d0, d1;

	__asm__ __volatile__(
		"cld\n\t"
		"rep ; stosl\n\t"
		"testb $2,%b3\n\t"
		"je 1f\n\t"
		"stosw\n"
		"1:\ttestb $1,%b3\n\t"
		"je 2f\n\t"
		"stosb\n"
		"2:"
		: "=&c" (d0), "=&D" (d1)
		: "a" (c), "q" (n), "0" (n/4), "1" ((long) s)
		: "memory"
	);
		
	return s;
}
#elif defined(__superh__)
static inline void *fb_arch_memset(void *s, int c, size_t n)
{
	__asm__ __volatile__ (
		"tst\tr6,r6\n"
		"bt/s\t5f\n"
	 	"add\tr6,r4\n"
		"mov\t#12,r0\n"
		"cmp/gt\tr6,r0\n"
		"bt/s\t4f\n"
	 	"mov\tr4,r0\n"
		"and\t#3,r0\n"
		"cmp/eq\t#0,r0\n"
		"bt/s\t2f\n"
		"sub\tr0,r6\n"
		"1:\n"
		"dt\tr0\n"
		"bf/s\t1b\n"
		"mov.b\tr5,@-r4\n"
		"2:\n"
		"swap.b\tr5,r0\n"
		"or\tr0,r5\n"
		"swap.w\tr5,r0\n"
		"or\tr0,r5\n"
		"mov\tr6,r0\n"
		"shlr2\tr0\n"
		"shlr\tr0\n"
		"3:\n"
		"dt\tr0\n"
		"mov.l\tr5,@-r4\n"
		"bf/s\t3b\n"
		"mov.l\tr5,@-r4\n"
		"mov\t#7,r0\n"
		"and\tr0,r6\n"
		"tst\tr6,r6\n"
		"bt\t5f\n"
		"4:\n"
		"dt\tr6\n"
		"bf/s\t4b\n"
	 	"mov.b\tr5,@-r4\n"
		"5:\n"
		"rts\n"
		"mov\tr4,r0\n"
		:
		: "a" (c), "q" (n), "0" (n/4), "1" ((long)s)
		: "memory"
	);

	return s;
}
#else
static inline void *fb_arch_memset(void *s, int c, size_t n)
{
  	while ((__u32)n--)
		*(__u16 *)s++ = (__u16)c;
	
	return s;
}
#endif

inline void *fb_memset(void *s, int c, size_t n)
{
	return fb_arch_memset(s, c, n);
}

inline void *fb_memmove(void *d, const void *s, size_t count)
{
	return fb_arch_memmove(d, s, count);
}
#endif // __FBASM_H_

