#include <gp2xregs.h>
#include <orcus.h>

volatile uint16_t* arm920Data = (r16) (((uint32_t)&__io_base)+DUAL920DAT0);
volatile uint16_t* arm940Data = (r16) (((uint32_t)&__io_base)+DUAL940DAT0);

bool arm940Running() {
  return REG16(DUALCTRL940) & BIT(7);
}

void arm940Halt() {
  REG16(DUALCTRL940) |= BIT(7);
}

void arm940Run() {
  REG16(DUALCTRL940) &= ~BIT(7);
}

extern void orcus_configure_peripherals();
void arm940Init(uint8_t memoryBank) {
  mmsp2PeripheralClockEnable.arm940t = true;
  orcus_configure_peripherals();
  REG16(DUALINT920) = 0x0;
  REG16(DUALINT940) = 0x0;

  REG16(DUALCTRL940) = (REG16(DUALCTRL940) & 0xFF) | BIT(7) | (memoryBank & 0x7F);
}
