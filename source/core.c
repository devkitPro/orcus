#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <gp2xregs.h>
#include <orcus.h>

extern void orcus_configure_display(bool isF200);
extern void orcus_init_syscalls();
extern void orcus_configure_peripherals();

r16 IO_BASE = (r16) 0xC0000000;

extern uint32_t __stack_base;
extern uint32_t __int_stack_size;
extern uint32_t __usr_stack_size;
uint32_t __heap_end;

void setClock(uint16_t value, uint16_t setVReg, uint16_t readVReg, uint16_t statusBit) {
  REG16(setVReg) = value;
  while(REG16(CLKCHGSTREG)&statusBit);
  for(int i = 0; (i<10000000) && (REG16(readVReg) != value) ; i++);
}

void gp2xInit() {
  // set up clock sources - default settings when system boots, set here in case something
  // else has messed around with them
  setClock(((F_MDIV << 8) | (F_PDIV << 2) | F_SDIV), FPLLSETVREG, FPLLVSETREG, CLKCHGSTREG_FPLLCHGST);
  setClock(((U_MDIV << 8) | (U_PDIV << 2) | U_SDIV), UPLLSETVREG, UPLLVSETREG, CLKCHGSTREG_UPLLCHGST);
  setClock(((A_MDIV << 8) | (A_PDIV << 2) | A_SDIV), APLLSETVREG, APLLVSETREG, CLKCHGSTREG_APLLCHGST);
  REG16(CLKMGRREG) = CLKMGRREG_APLL_USE | CLKMGRREG_UPLL_USE;

  // configure syscalls
  orcus_init_syscalls();

  // disable clocks for unwanted peripherals
  orcus_configure_peripherals();

  // set up UART0 to 115200/8N1
  uartConfigure(115200, 8, NONE, 1);

  // set up memory timings
  orcus_default_ram_timings();

  // check if this is an f200
  bool isF200 = gp2xIsF200();

  orcus_configure_gpio(isF200);
  orcus_configure_display(isF200);

  // TODO understand the interrupt subsystem, seems pretty simple, jut specify fiq/irq for each type, and there is a register which the ISR can read to see what caused it - chapter 8 'interrupt controller'

  // establish memory map - TODO - this formula isn't correct for some reason, we should be getting 0x3FFDF00
  __heap_end = __stack_base - __int_stack_size - __int_stack_size - __usr_stack_size;
  uart_printf("heapend: 0x%08x\r\n", __heap_end);

  extern void* heap_ptr;
  heap_ptr = (void*)&__heap_start;
}

void orcus_set_ram_timings(int tRC, int tRAS, int tWR, int tMRD, int tRFC, int tRP, int tRCD) {
  REG16(MEMTIMEX0) = ((tMRD & 0xF) << 12) | ((tRFC & 0xF) << 8) | ((tRP & 0xF) << 4) | (tRCD & 0xF);
  REG16(MEMTIMEX1) = ((tRC & 0xF) << 8) | ((tRAS & 0xF) << 4) | (tWR & 0xF);  
}

void orcus_default_ram_timings() { orcus_set_ram_timings(7, 15, 2, 7, 7, 7, 7); }
void orcus_fast_ram_timings() { orcus_set_ram_timings(5, 3, 0, 0, 0, 1, 1); }

void orcus_delay(int loops) {
  volatile int i = loops;
  for(;i != 0;i--);
}

_mmsp2_peripheral_clock_enable mmsp2PeripheralClockEnable = {
							     .externalSystemClock = false,
							     .adc = true,
							     .pwm = false,
							     .dma = true,
							     .timer = true,
							     .coprocessor = false,
							     .gpio = true,
							     .memC = true,
							     .memA = true,
							     .arm940t = false,
							     .ssp = false,
							     .i2c = true,
							     .uart3 = false,
							     .uart2 = false,
							     .uart1 = false,
							     .uart0 = true,
							     .usbDeviceController = false,
							     .usbHostController = false,
							     .deblock = false,
							     .mpeg = false,
							     .motionEstimation = false,
							     .refWrite = false,
							     .rvld = false,
							     .qpelGmc = false,
							     .mpMc = false,
							     .pdf = false,
							     .huff = false,
							     .vpGoff = false,
							     .vpTop = false,
							     .isp = false,
							     .grp = false,
							     .scale = false,
							     .disp = true,
							     .cdrom = false,
							     .ide = false,
							     .memoryStick = false,
							     .mmc = true,
							     .spdifOut = false,
							     .spdifIn = false,
							     .i2s = false,
							     .ac97 = true };

void orcus_configure_peripherals() {
  REG16(SYSCLKENREG) = SET(REG16(SYSCLKENREG), SYSCLKENREG_ESYSCLK, mmsp2PeripheralClockEnable.externalSystemClock);
  REG16(SYSCLKENREG) = SET(REG16(SYSCLKENREG), SYSCLKENREG_ADCCLK, mmsp2PeripheralClockEnable.adc);
  REG16(SYSCLKENREG) = SET(REG16(SYSCLKENREG), SYSCLKENREG_PWMCLK, mmsp2PeripheralClockEnable.pwm);
  REG16(SYSCLKENREG) = SET(REG16(SYSCLKENREG), SYSCLKENREG_DMACLK, mmsp2PeripheralClockEnable.dma);
  REG16(SYSCLKENREG) = SET(REG16(SYSCLKENREG), SYSCLKENREG_TIMER, mmsp2PeripheralClockEnable.timer);
  REG16(SYSCLKENREG) = SET(REG16(SYSCLKENREG), SYSCLKENREG_PERI, mmsp2PeripheralClockEnable.coprocessor);
  REG16(SYSCLKENREG) = SET(REG16(SYSCLKENREG), SYSCLKENREG_GPIO, mmsp2PeripheralClockEnable.gpio);
  REG16(SYSCLKENREG) = SET(REG16(SYSCLKENREG), SYSCLKENREG_MEMCCLK, mmsp2PeripheralClockEnable.memC);
  REG16(SYSCLKENREG) = SET(REG16(SYSCLKENREG), SYSCLKENREG_MEMACLK, mmsp2PeripheralClockEnable.memA);
  REG16(SYSCLKENREG) = SET(REG16(SYSCLKENREG), SYSCLKENREG_A940TCLK, mmsp2PeripheralClockEnable.arm940t);  
  REG16(COMCLKENREG) = SET(REG16(COMCLKENREG), COMCLKENREG_SSPCLK, mmsp2PeripheralClockEnable.ssp);
  REG16(COMCLKENREG) = SET(REG16(COMCLKENREG), COMCLKENREG_I2CCLK, mmsp2PeripheralClockEnable.i2c);
  REG16(COMCLKENREG) = SET(REG16(COMCLKENREG), COMCLKENREG_UART3CLK, mmsp2PeripheralClockEnable.uart3);
  REG16(COMCLKENREG) = SET(REG16(COMCLKENREG), COMCLKENREG_UART2CLK, mmsp2PeripheralClockEnable.uart2);
  REG16(COMCLKENREG) = SET(REG16(COMCLKENREG), COMCLKENREG_UART1CLK, mmsp2PeripheralClockEnable.uart1);
  REG16(COMCLKENREG) = SET(REG16(COMCLKENREG), COMCLKENREG_UART0CLK, mmsp2PeripheralClockEnable.uart0);
  REG16(COMCLKENREG) = SET(REG16(COMCLKENREG), COMCLKENREG_UDCCLK, mmsp2PeripheralClockEnable.usbDeviceController);
  REG16(COMCLKENREG) = SET(REG16(COMCLKENREG), COMCLKENREG_UHCCLK, mmsp2PeripheralClockEnable.usbHostController);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_DBLKCLK, mmsp2PeripheralClockEnable.deblock);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_MPGIFCLK, mmsp2PeripheralClockEnable.mpeg);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_MECLK, mmsp2PeripheralClockEnable.motionEstimation);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_REFWCLK, mmsp2PeripheralClockEnable.refWrite);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_RVLDCLK, mmsp2PeripheralClockEnable.rvld);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_QGMCCLK, mmsp2PeripheralClockEnable.qpelGmc);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_MPMCCLK, mmsp2PeripheralClockEnable.mpMc);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_PDFCLK, mmsp2PeripheralClockEnable.pdf);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_HUFFCLK, mmsp2PeripheralClockEnable.huff);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_VPCLKGOFF, mmsp2PeripheralClockEnable.vpGoff);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_VPTOPCLK, mmsp2PeripheralClockEnable.vpTop);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_ISPCLK, mmsp2PeripheralClockEnable.isp);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_GRPCLK, mmsp2PeripheralClockEnable.grp);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_SCALECLK, mmsp2PeripheralClockEnable.scale);
  REG16(VGCLKENREG) = SET(REG16(VGCLKENREG), VGCLKENREG_DISPCLK, mmsp2PeripheralClockEnable.disp);
  REG16(ASCLKENREG) = SET(REG16(ASCLKENREG), ASCLKENREG_CDROMCLK, mmsp2PeripheralClockEnable.cdrom);
  REG16(ASCLKENREG) = SET(REG16(ASCLKENREG), ASCLKENREG_IDECLK, mmsp2PeripheralClockEnable.ide);
  REG16(ASCLKENREG) = SET(REG16(ASCLKENREG), ASCLKENREG_MSTICKCLK, mmsp2PeripheralClockEnable.memoryStick);
  REG16(ASCLKENREG) = SET(REG16(ASCLKENREG), ASCLKENREG_MMCCLK, mmsp2PeripheralClockEnable.mmc);
  REG16(ASCLKENREG) = SET(REG16(ASCLKENREG), ASCLKENREG_SPDIFOCLK, mmsp2PeripheralClockEnable.spdifOut);
  REG16(ASCLKENREG) = SET(REG16(ASCLKENREG), ASCLKENREG_SPDIFICLK, mmsp2PeripheralClockEnable.spdifIn);
  REG16(ASCLKENREG) = SET(REG16(ASCLKENREG), ASCLKENREG_I2SCLK, mmsp2PeripheralClockEnable.i2s);
  REG16(ASCLKENREG) = SET(REG16(ASCLKENREG), ASCLKENREG_AC97CLK, mmsp2PeripheralClockEnable.ac97);
}

uint32_t btnState() {
  uint16_t c = ~REG16(GPIOCPINLVL);
  uint16_t d = ~REG16(GPIODPINLVL);
  uint16_t m = ~REG16(GPIOMPINLVL);
  return (c & (1<<10) ? L : 0)
    | (c & (1<<11) ? R : 0)
    | (c & (1<<15) ? Y : 0)
    | (c & (1<<12) ? A : 0)
    | (c & (1<<13) ? B : 0)
    | (c & (1<<14) ? X : 0)
    | (c & (1<<8) ? START : 0)
    | (c & (1<<9) ? SELECT : 0)
    | (m & (1<<0) ? UP : 0)
    | (m & (1<<1) ? UP_LEFT : 0)
    | (m & (1<<2) ? LEFT : 0)
    | (m & (1<<3) ? DOWN_LEFT : 0)
    | (m & (1<<4) ? DOWN : 0)
    | (m & (1<<5) ? DOWN_RIGHT : 0)
    | (m & (1<<6) ? RIGHT : 0)
    | (m & (1<<7) ? UP_RIGHT : 0)
    | (d & (1<<11) ? STICK : 0)
    | (d & (1<<6) ? VOL_DOWN : 0)
    | (d & (1<<7) ? VOL_UP : 0);    
}

bool gp2xIsF200() {
  return true;
}
