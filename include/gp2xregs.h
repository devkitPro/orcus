#ifndef __GP2X_REGS_H__
#define __GP2X_REGS_H__

#include <stdint.h>

#define r32 volatile uint32_t*
#define r16 volatile uint16_t*
#define r8 volatile uint8_t*

extern r16 IO_BASE;

#define REG16(x) *((r16) IO_BASE + x)
#define BIT(x) (1 << x)

// interrupt vector
extern r32 _undefined_instruction;
extern r32 _software_interrupt;
extern r32 _prefetch_abort;
extern r32 _data_abort;
extern r32 _not_used;
extern r32 _irq;
extern r32 _fiq;

#define F_MDIV 0x49
#define F_PDIV 0x1
#define F_SDIV 0x0
#define U_MDIV 0x60
#define U_PDIV 0x0
#define U_SDIV 0x2
#define A_MDIV 0x98
#define A_PDIV 0x0
#define A_SDIV 0x2

#define APLL_CLK 0x3
#define UPLL_CLK 0x2
#define FPLL_CLK 0x1
#define NO_CLK 0x0

#define FPLLSETVREG 0x0910
#define UPLLSETVREG 0x0914
#define APLLSETVREG 0x0918

#define CLKMGRREG 0x0C12
#define CLKMGRREG_APLL_USE BIT(1)
#define CLKMGRREG_UPLL_USE BIT(0)

#define URT0CSETREG 0x928
#define URT1CSETREG 0x92A
#define URT0CSETREG_UART0(x) (x << 0)
#define URT0CSETREG_UART1(x) (x << 8)
#define URT1CSETREG_UART2(x) (x << 0)
#define URT1CSETREG_UART3(x) (x << 8)
#define URTnCSETREG_CLKSRC(x) (x << 6)
#define URTnCSETREG_CLKDIV(x) x

#endif
