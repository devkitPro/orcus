#ifndef __GP2X_REGS_H__
#define __GP2X_REGS_H__

#include <stdint.h>
#include <stdbool.h>

#define r32 volatile uint32_t*
#define r16 volatile uint16_t*
#define r8 volatile uint8_t*

extern volatile void* __io_base;
#define REG8(x) *((r8) (((uint32_t)&__io_base)+x))
#define REG16(x) *((r16) (((uint32_t)&__io_base)+x))
#define REG32(x) *((r32) (((uint32_t)&__io_base)+x))

extern void* __nand_io_base;
#define NANDREG8(x) *((r8) (((uint32_t)&__nand_io_base)+x))
#define NANDREG16(x) *((r16) (((uint32_t)&__nand_io_base)+x))

#define FREG8(x) *((r8) (((uint32_t)&__io_base)+x+0x20000000))
#define FREG16(x) *((r16) (((uint32_t)&__io_base)+x+0x20000000))
#define FREG32(x) *((r32) (((uint32_t)&__io_base)+x+0x20000000))

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
#define MEMTIMEW0 0x3A02
#define MEMTIMEW1 0x3A04
#define MEMTIMEW2 0x3A06
#define MEMTIMEW4 0x3A0A

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
#define GPIOAALTFNLOW 0x1020
#define GPIOAALTFNHI 0x1040
#define GPIOBALTFNLOW 0x1022
#define GPIOBALTFNHI 0x1042
#define GPIODALTFNLOW 0x1026
#define GPIOEALTFNLOW 0x1028
#define GPIOEALTFNHI 0x1048
#define GPIOHALTFNLOW 0x102E
#define GPIOHALTFNHI 0x104E
#define GPIOIALTFNHI 0x1050
#define GPIOJALTFNHI 0x1052
#define GPIOKALTFNLOW 0x1034
#define GPIOLALTFNLOW 0x1036
#define GPIOLALTFNHI 0x1056
#define GPIOFALTFNLOW 0x102A
#define GPIOGALTFNLOW 0x102C
#define GPIOGALTFNHI 0x102C

#define GPIOAOUT 0x1060
#define GPIOBOUT 0x1062
#define GPIOCOUT 0x1064
#define GPIODOUT 0x1066
#define GPIOEOUT 0x1068
#define GPIOFOUT 0x106A
#define GPIOGOUT 0x106C
#define GPIOHOUT 0x106E
#define GPIOIOUT 0x1070
#define GPIOJOUT 0x1072
#define GPIOKOUT 0x1074
#define GPIOLOUT 0x1076
#define GPIOMOUT 0x1078
#define GPIONOUT 0x107A
#define GPIOOOUT 0x107C

#define GPIOAPINLVL 0x1180
#define GPIOBPINLVL 0x1182
#define GPIOCPINLVL 0x1184
#define GPIODPINLVL 0x1186
#define GPIOEPINLVL 0x1188
#define GPIOFPINLVL 0x118A
#define GPIOGPINLVL 0x118C
#define GPIOHPINLVL 0x118E
#define GPIOIPINLVL 0x1190
#define GPIOJPINLVL 0x1192
#define GPIOKPINLVL 0x1194
#define GPIOLPINLVL 0x1196
#define GPIOMPINLVL 0x1198
#define GPIONPINLVL 0x119A
#define GPIOOPINLVL 0x119C

#define GPIOBPUENB 0x10C2
#define GPIOIPUENB 0x10D0
#define GPIOLPUENB 0x10D6

#define IN 0x0
#define OUT 0x1
#define ALTFN1 0x2
#define ALTFN2 0x3

/* GPIOs used by program code */

// GPIOB
#define CX25874_SLEEP BIT(2) // sleep = 1, awake = 0
#define LCD_RESET BIT(3)

// GPIOC
#define BTN_START BIT(8)
#define BTN_SELECT BIT(9)
#define BTN_SHOULDER_LEFT BIT(10)
#define BTN_SHOULDER_RIGHT BIT(11)
#define BTN_A BIT(12)
#define BTN_B BIT(13)
#define BTN_X BIT(14)
#define BTN_Y BIT(15)

// GPIOD
#define BTN_VOL_DOWN BIT(6)
#define BTN_VOL_UP BIT(7)
#define BTN_STICK_PRESS BIT(11)

// GPIOH
#define LCD_VGH BIT(1)
#define BACKLIGHT_F100 BIT(2)
#define BATTERY_LED BIT(4) 
#define BACKLIGHT_F200 BIT(11)

// GPIOM
#define STK_UP BIT(0)
#define STK_UP_LEFT BIT(1)
#define STK_LEFT BIT(2)
#define STK_DOWN_LEFT BIT(3)
#define STK_DOWN BIT(4)
#define STK_DOWN_RIGHT BIT(5)
#define STK_RIGHT BIT(6)
#define STK_UP_RIGHT BIT(7)

// display registers
#define DISPCSETREG 0x0924
#define DISPCLKSRC(x) (x << 14)
#define DISPCLKDIV(x) (x << 8)
#define DISPCLKPOL(x) (x << 7)

#define DPC_CNTL 0x2800
#define PAL(x) (x << 12)
#define CISCYNC(x) (x << 11)
#define HDTV(x) (x << 10)
#define ENC(x) (x << 9)
#define TVMODE(x) (x << 8)
#define DOT(x) (x << 6)
#define INTERLACE(x) (x << 5)
#define SYNCCBCR(x) (x << 4)
#define ESAVEN(x) (x << 3)
#define DOF(x) (x << 1)
#define ENB(x) (x << 0)

#define DPC_CLKCNTL 0x2848
#define CLKSRC(x) (x << 3)
#define CLK2SEL(x) (x << 2)
#define CLK1SEL(x) (x << 1)
#define CLK1POL(x) (x << 0)

#define DPC_X_MAX 0x2816
#define DPC_Y_MAX 0x2818

#define DPC_HS_WIDTH 0x281A
#define HSPADCFG(x) (x << 8)
#define HSWID(x) (x << 0)

#define DPC_HS_STR 0x281C
#define DPC_HS_END 0x281E

#define DPC_DE 0x2826
#define DEPADCFG(x) (x << 14)
#define DESTR(x) (x << 4)
#define DEWID(x) (x << 0)

#define DPC_FPIPOL1 0x2804
#define DPC_FPIPOL2 0x2806
#define DPC_FPIPOL3 0x2808
#define HSPOL(x) (x << 1)

#define DPC_V_SYNC 0x2820
#define DPC_V_END 0x2822

#define DPC_FPIATV1 0x280A
#define DPC_FPIATV2 0x280C
#define DPC_FPIATV3 0x280E

#define MLC_STL_CNTL 0x28DA
#define MLC_STL_BPP(x) (x << 9)

#define MLC_STL_MIXMUX 0x28DC
#define MLC_STL_ALPHAL 0x28DE
#define MLC_STL_ALPHAH 0x28E0

#define MLC_STL_CKEY_GR 0x2902
#define MLC_STL_CKEY_B 0x2904

#define MLC_STL_HSC 0x2906
#define MLC_STL_VSCL 0x2908
#define MLC_STL_VSCH 0x290A
#define MLC_STL_HW 0x290C

#define MLC_STL_OADRL 0x290E
#define MLC_STL_OADRH 0x2910
#define MLC_STL_EADRL 0x2912
#define MLC_STL_EADRH 0x2914

#define MLC_STLn_STX 0x28E2

#define MLC_OVLAY_CNTR 0x2880

#define SDICON 0x1500
#define SDICON_BYT_ORDER (1 << 4)
#define SDICON_ENCLK(x) (x << 0)
#define SDIPRE 0x1502
#define SDICmdArgL 0x1504
#define SDICmdArgH 0x1506
#define SDICmdCon 0x1508
#define SDICmdCon_HOST_RCV(x) (x << 10) // long response = 1, short = 0
#define SDICmdCon_HOST_WAITS(x) (x << 9)
#define SDICmdCon_CMD_OP_ST(x) (x << 8)
#define SDICmdCon_CMD_INDEX(x) ((0x2 << 7) | x)

#define SDICmdSta 0x150A
#define SDICmdSta_RSP_CRC (1 << 12)
#define SDICmdSta_CMD_SENT (1 << 11)
#define SDICmdSta_CMD_TOUT (1 << 10)
#define SDICmdSta_RSP_FIN (1 << 9)
#define SDICmdSta_CMD_ON (1 << 8)

#define SDIDatConL 0x1520
#define SDIDatConH 0x1522
#define SDIDatConH_WIDE_BUS(x) (x << 0)
#define SDIDatConH_BLK_MODE(x) (x << 1)
#define SDIDatConH_RACMD(x) (x << 3)

#define SDIRSP0 0x150C
#define SDIRSP1 0x150E
#define SDIRSP2 0x1510
#define SDIRSP3 0x1512
#define SDIRSP4 0x1514
#define SDIRSP5 0x1516
#define SDIRSP6 0x1518
#define SDIRSP7 0x151A

#define SDIBSize 0x151E

#define SDIFSTA 0x152A
#define SDIDatSta 0x1528
#define SDIDAT 0x152C

#define SDIDatCnt 0x1524 // 32bit

#define SDIDTimerL 0x1536
#define SDIDTimerH 0x1538

#define TCOUNT 0x0A00
#define TCONTROL 0x0A14

#define MEMNANDCTRLW 0x3A3A
#define MEMNANDTIMEW 0x3A3C

#define NFDATA 0x0000
#define NFCMD 0x0010
#define NFADDR 0x0018

#define DUAL920DAT0 0x3B00
#define DUAL940DAT0 0x3B20
#define DUALINT920 0x3B40
#define DUALINT940 0x3B42
#define DUALCTRL940 0x3B48

typedef struct {
  // SYSCLKENREG
  bool externalSystemClock: 1;
  bool adc: 1;
  bool pwm: 1;
  bool dma: 1;
  bool timer: 1;
  bool coprocessor: 1;
  bool gpio: 1;
  bool memC: 1;
  bool memA: 1;
  bool arm940t: 1;

  // COMCLKENREG
  bool ssp: 1;
  bool i2c: 1;
  bool uart3: 1;
  bool uart2: 1;
  bool uart1: 1;
  bool uart0: 1;
  bool usbDeviceController: 1;
  bool usbHostController: 1;

  // VGCLKENREG
  bool deblock: 1;
  bool mpeg: 1;
  bool motionEstimation: 1;
  bool refWrite: 1;
  bool rvld: 1;
  bool qpelGmc: 1;
  bool mpMc: 1;
  bool pdf: 1;
  bool huff: 1;
  bool vpGoff: 1;
  bool vpTop: 1;
  bool isp: 1;
  bool grp: 1;
  bool scale: 1;
  bool disp: 1;

  // ASCLKENREG
  bool cdrom: 1;
  bool ide: 1;
  bool memoryStick: 1;
  bool mmc: 1;
  bool spdifOut: 1;
  bool spdifIn: 1;
  bool i2s: 1;
  bool ac97: 1;
} _mmsp2_peripheral_clock_enable;

#define DMACOM0 0x0200
#define DMACOM1 0x0202
#define DMACOM2 0x0204
#define DMACONS 0x0206
#define DMASRCADDR 0x0208
#define DMATRGADDR 0x020C
#define DMAREG(reg, channel) (reg+(0x10*channel))

#define DCH0SRM 0x0100
#define DCH0TRM 0x0102

#define AC_CTRL_REG 0x0E00
#define AC_STA_ENA_REG 0x0E04
#define AC_STATUS_REG 0x0E06
#define AC_CODEC_DATA 0x0E0E
#define AC_CODEC_ACCESS 0x0E10

// FREG
#define DSTCTRL 0x24000
#define DSTADDR 0x24004
#define DSTSTRIDE 0x24008
#define SRCCTRL 0x2400C
#define SRCADDR 0x24010
#define SRCSTRIDE 0x24014
#define SRCFORCOLOR 0x24018
#define SRCBACKCOLOR 0x2401C
#define SIZE 0x2402C
#define CTRL 0x24030
#define RUN 0x24034
#define PATCTRL 0x24020
#define PATFORCOLOR 0x24024
#define PATBACKCOLOR 0x24028
#define SRCFORCOLOR 0x24018
#define SRCBACKCOLOR 0x2401C
#define PAT 0x24080
#define ROT_CNTL 0x24100
#define ROT_SRCSTRIDE 0x24104
#define ROT_DSTSTRIDE 0x24108
#define ROT_PICSIZE 0x2410C
#define ROT_SRCADDR 0x24110
#define ROT_DSTADDR 0x24114

#endif
