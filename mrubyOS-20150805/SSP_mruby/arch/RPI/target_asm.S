#define TOPPERS_MACRO_ONLY
#define TOPPERS_ASM_MACRO
#define UINT_C(val)     (val)        /* uint_t型の定数を作るマクロ */
#define ULONG_C(val)    (val)        /* ulong_t型の定数を作るマクロ */

#include "target_asm.inc"

// IRQ有効化（CPSR設定）
	.global enable_IRQ
	.global enable_irq
enable_IRQ:
enable_irq:
	mrs	r0, cpsr
	bic r0, r0, #0x80
	msr cpsr_c,r0
	bx lr

// IRQ無効化（CPSR設定）
	.global disable_IRQ
disable_IRQ:
	mrs	r0, cpsr
	ldr	r1,	=0x80
	orr r0, r0, r1
	msr	cpsr_c, r0
	bx lr


.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

