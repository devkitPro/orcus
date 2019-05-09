#include <stdint.h>
#include <gp2xregs.h>
#include <orcus.h>

r16 IO_BASE = (r16) 0xC0000000;

extern uint32_t __stack_base;
extern uint32_t __int_stack_size;
extern uint32_t __usr_stack_size;
uint32_t __heap_end;

// TODO - sys/iosupport.h, __syscalls need s

int calculateUartDiviser(int baudRate) {
  const int clkBasis = 18432000;
  return (int) (clkBasis / (baudRate * 16)) - 1;
}

void orcus_init() {
  // establish memory map
  __heap_end = __stack_base - __int_stack_size - __int_stack_size - __usr_stack_size;
  
  // set up clock sources - default settings when system boots, set here in case something
  // else has messed around with them
  REG16(FPLLSETVREG) = ((F_MDIV << 8) & (F_PDIV << 2) & F_SDIV);
  REG16(UPLLSETVREG) = ((U_MDIV << 8) & (U_PDIV << 2) & U_SDIV);
  REG16(APLLSETVREG) = ((A_MDIV << 8) & (A_PDIV << 2) & A_SDIV);
  REG16(CLKMGRREG) = CLKMGRREG_APLL_USE | CLKMGRREG_UPLL_USE;

  // set up UART0 to 115200 bps
  REG16(URT0CSETREG) = URT0CSETREG_UART0(URTnCSETREG_CLKSRC(APLL_CLK) | URTnCSETREG_CLKDIV(calculateUartDiviser(115200)));

  // TODO - disable clocks for unwanted peripherals
  //  	clk_power->SYSCLKENREG &= ~(ESYSCLK|PWMCLK|FASTIOCLK|A940TCLK);
	//	clk_power->COMCLKENREG &= ~(SSPCLK|UART3CLK|UART2CLK|UART1CLK|OWMCLK|UDCCLK|UHCCLK);
	//	clk_power->VGCLKENREG  &= ~(DBLKCLK|MPGIFCLK|DECSSCLK|MECLK|REFWCLK|RVLDCLK|QGMCLK|MPMCLK|PDFCLK
				    //								|HUFFCLK|VPCLKGOFF|VPTOPCLK|ISPCLK|GRPCLK|SCALECLK);
	//	clk_power->ASCLKENREG  &= ~(CDROMCLK|IDECLK|MSTICKCLK|SPDIFOCLK|SPDIFICLK|I2SCLK);
  
  // TODO set up memory settings, and __syscalls for malloc in newlib

  // TODO understand the interrupt subsystem, seems pretty simple, jut specify fiq/irq for each type, and there is a register which the ISR can read to see what caused it - chapter 8 'interrupt controller'
}
