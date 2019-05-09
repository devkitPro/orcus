#ifndef __GP2X_REGS_H__
#define __GP2X_REGS_H__

#include <stdint.h>
#include <stdbool.h>

#define r32 volatile uint32_t*
#define r16 volatile uint16_t*
#define r8 volatile uint8_t*

extern r16 IO_BASE;

#define REG16(x) *((r16) IO_BASE + x)
#define BIT(x) (1 << x)
#define SET(reg, bit, onOff) ((reg&(~bit))|(onOff ? bit : 0))

// interrupt vector
extern r32 _undefined_instruction;
extern r32 _software_interrupt;
extern r32 _prefetch_abort;
extern r32 _data_abort;
extern r32 _not_used;
extern r32 _irq;
extern r32 _fiq;

// clock registers
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

#define SYSCLKENREG 0x0904
#define SYSCLKENREG_ESYSCLK BIT(14)
#define SYSCLKENREG_ADCCLK BIT(13)
#define SYSCLKENREG_PWMCLK BIT(12)
#define SYSCLKENREG_DMACLK BIT(9)
#define SYSCLKENREG_TIMER BIT(7)
#define SYSCLKENREG_PERI BIT(6)
#define SYSCLKENREG_GPIO BIT(5)
#define SYSCLKENREG_MEMCCLK BIT(3)
#define SYSCLKENREG_MEMACLK BIT(1)
#define SYSCLKENREG_A940TCLK BIT(0)

#define COMCLKENREG 0x0908
#define COMCLKENREG_SSPCLK BIT(9)
#define COMCLKENREG_I2CCLK BIT(8)
#define COMCLKENREG_UART3CLK BIT(7)
#define COMCLKENREG_UART2CLK BIT(6)
#define COMCLKENREG_UART1CLK BIT(5)
#define COMCLKENREG_UART0CLK BIT(4)
#define COMCLKENREG_UDCCLK BIT(1)
#define COMCLKENREG_UHCCLK BIT(0)

#define VGCLKENREG 0x090A
#define VGCLKENREG_DBLKCLK BIT(15)
#define VGCLKENREG_MPGIFCLK BIT(14)
#define VGCLKENREG_MECLK BIT(12)
#define VGCLKENREG_REFWCLK BIT(11)
#define VGCLKENREG_RVLDCLK BIT(10)
#define VGCLKENREG_QGMCCLK BIT(9)
#define VGCLKENREG_MPMCCLK BIT(8)
#define VGCLKENREG_PDFCLK BIT(7)
#define VGCLKENREG_HUFFCLK BIT(6)
#define VGCLKENREG_VPCLKGOFF BIT(5)
#define VGCLKENREG_VPTOPCLK BIT(4)
#define VGCLKENREG_ISPCLK BIT(3)
#define VGCLKENREG_GRPCLK BIT(2)
#define VGCLKENREG_SCALECLK BIT(1)
#define VGCLKENREG_DISPCLK BIT(0)

#define ASCLKENREG 0x090C
#define ASCLKENREG_CDROMCLK BIT(13)
#define ASCLKENREG_IDECLK BIT(12)
#define ASCLKENREG_MSTICKCLK BIT(9)
#define ASCLKENREG_MMCCLK BIT(8)
#define ASCLKENREG_SPDIFOCLK BIT(3)
#define ASCLKENREG_SPDIFICLK BIT(2)
#define ASCLKENREG_I2SCLK BIT(1)
#define ASCLKENREG_AC97CLK BIT(0)

#define MEMTIMEX0 0x3802
#define MEMTIMEX1 0x3804

#endif
