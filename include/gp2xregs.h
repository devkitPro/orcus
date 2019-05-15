#ifndef __GP2X_REGS_H__
#define __GP2X_REGS_H__

#include <stdint.h>
#include <stdbool.h>

#define r32 volatile uint32_t*
#define r16 volatile uint16_t*
#define r8 volatile uint8_t*

extern r16 IO_BASE;

#define EG(offset, size) ((volatile r##size) (0xC0000000+offset))
#define REG8(x) *((volatile r8) (0xC0000000+x))
#define REG16(x) *((volatile r16) (0xC0000000+x))
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

#define REGISTER(name, offset, size) volatile r##size name = (r##size) (0xC0000000+offset)

// clock registers
#define SYSCSETREG 0x091C
#define SYSCSETREG_920(x) (x<<0)
#define SYSCSETREG_940(x) (x<<3)

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
#define FPLLVSETREG 0x0912
#define UPLLSETVREG 0x0914
#define UPLLVSETREG 0x0916
#define APLLSETVREG 0x0918
#define APLLVSETREG 0x091A
  
#define CLKCHGSTREG 0x0902
#define CLKCHGSTREG_FPLLCHGST BIT(0)
#define CLKCHGSTREG_UPLLCHGST BIT(1)
#define CLKCHGSTREG_APLLCHGST BIT(1)

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

#define LCON0 0x1200
#define LCON1 0x1220
#define LCON2 0x1240
#define LCON3 0x1260
#define LCONx_SIR_MODE(x) (x << 6)
#define LCONx_SIR_MODE_NORMAL 0x0
#define LCONx_SIR_MODE_INFRARED 0x1
#define LCONx_PARITY(x) (x << 3)
#define LCONx_PARITY_NONE 0x0
#define LCONx_PARITY_ODD 0x4
#define LCONx_PARITY_EVEN 0x5
#define LCONx_STOPBIT(x) (x << 2)
#define LCONx_STOPBIT_ONE 0x0
#define LCONx_STOPBIT_TWO 0x1
#define LCONx_WORD_LEN(x) (x << 0)
#define LCONx_WORD_LEN_5BITS 0x0
#define LCONx_WORD_LEN_6BITS 0x1
#define LCONx_WORD_LEN_7BITS 0x2
#define LCONx_WORD_LEN_8BITS 0x3

#define UCON0 0x1202
#define UCON1 0x1222
#define UCON2 0x1242
#define UCON3 0x1262
#define UCONx_TRANS_MODE(x) (x << 2)
#define UCONx_RECEIVE_MODE(x) (x << 0)
#define UCONx_MODE_DISABLE 0x0
#define UCONx_MODE_INTPOLL 0x1
#define UCONx_MODE_DMA 0x2

#define FCON0 0x1204  
#define FCON1 0x1224  
#define FCON2 0x1244  
#define FCON3 0x1264  
#define FCONx_TX_FIFO_RESET BIT(2)
#define FCONx_RX_FIFO_RESET BIT(1)
#define FCONx_FIFO_EN BIT(0)

#define MCON0 0x1206

#define BRD0 0x1214

#define ESTATUS0 0x120A
#define ESTATUS1 0x122A
#define ESTATUS2 0x124A
#define ESTATUS3 0x126A
#define ESTATUSx_BREAK_DETECT BIT(3)
#define ESTATUSx_FRAME_ERROR BIT(2)
#define ESTATUSx_PARITY_ERROR BIT(1)
#define ESTATUSx_OVERRUN_ERROR BIT(0)

#define FSTATUS0 0x120C
#define FSTATUS1 0x122C
#define FSTATUS2 0x124C
#define FSTATUS3 0x126C
#define FSTATUSx_RX_FIFO_ERROR BIT(10)
#define FSTATUSx_TX_FIFO_FULL BIT(9)
#define FSTATUSx_RX_FIFO_FULL BIT(8)
#define FSTATUSx_TX_FIFO_COUNT(reg) ((REG16(reg)&0xF0)>>4)
#define FSTATUSx_RX_FIFO_COUNT(reg) (REG16(reg)&0x0F)

// transmit for UARTn - 8 bits
#define THB0 0x1210
#define THB1 0x1230
#define THB2 0x1250
#define THB3 0x1270
// receive for UARTn - 8 bits
#define RHB0 0x1212
#define RHB1 0x1232
#define RHB2 0x1252
#define RHB3 0x1272

// GPIO
#define GPIODALTFNLOW 0x1026
#define GPIODALTFNHI 0x1046
  
#endif
