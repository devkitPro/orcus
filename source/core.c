#include <stdint.h>
#include <gp2xregs.h>
#include <orcus.h>

r16 IO_BASE = (r16) 0xC0000000;

extern uint32_t __stack_base;
extern uint32_t __int_stack_size;
extern uint32_t __usr_stack_size;
uint32_t __heap_end;

_mmsp2_peripheral_clock_enable mmsp2PeripheralClockEnable = {
							     .externalSystemClock = false,
							     .adc = true,
							     .pwm = true,
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

void orcus_init() {  
  // set up clock sources - default settings when system boots, set here in case something
  // else has messed around with them
  REG16(FPLLSETVREG) = ((F_MDIV << 8) & (F_PDIV << 2) & F_SDIV);
  REG16(UPLLSETVREG) = ((U_MDIV << 8) & (U_PDIV << 2) & U_SDIV);
  REG16(APLLSETVREG) = ((A_MDIV << 8) & (A_PDIV << 2) & A_SDIV);
  REG16(CLKMGRREG) = CLKMGRREG_APLL_USE | CLKMGRREG_UPLL_USE;

  // set up UART0 to 115200 bps
  REG16(URT0CSETREG) = URT0CSETREG_UART0(URTnCSETREG_CLKSRC(APLL_CLK) | URTnCSETREG_CLKDIV(orcus_calculate_uart_diviser(115200)));

  // disable clocks for unwanted peripherals
  orcus_configure_peripherals();
  
  // set up memory timings
  orcus_default_ram_timings();

  // TODO understand the interrupt subsystem, seems pretty simple, jut specify fiq/irq for each type, and there is a register which the ISR can read to see what caused it - chapter 8 'interrupt controller'

  // establish memory map
  __heap_end = __stack_base - __int_stack_size - __int_stack_size - __usr_stack_size;
  // TODO __syscalls for malloc in newlib
  // TODO - sys/iosupport.h, __syscalls need s

}

int orcus_calculate_uart_diviser(int baudRate) {
  const int clkBasis = 18432000;
  return (int) (clkBasis / (baudRate * 16)) - 1;
}

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

void orcus_set_ram_timings(int tRC, int tRAS, int tWR, int tMRD, int tRFC, int tRP, int tRCD) {
  REG16(MEMTIMEX0) = ((tMRD & 0xF) << 12) | ((tRFC & 0xF) << 8) | ((tRP & 0xF) << 4) | (tRCD & 0xF);
  REG16(MEMTIMEX1) = ((tRC & 0xF) << 8) | ((tRAS & 0xF) << 4) | (tWR & 0xF);  
}

void orcus_default_ram_timings() { orcus_set_ram_timings(7, 15, 2, 7, 7, 7, 7); }
void orcus_fast_ram_timings() { orcus_set_ram_timings(5, 3, 0, 0, 0, 1, 1); }
