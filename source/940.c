#include <gp2xregs.h>
#include <orcus.h>

volatile uint16_t* arm920Data = (r16) (((uint32_t)&__io_base)+DUAL920DAT0);
volatile uint16_t* arm940Data = (r16) (((uint32_t)&__io_base)+DUAL940DAT0);

extern void* __is_arm940;
extern void* __arm940_bank;

bool arm940Running() {
  return !(REG16(DUALCTRL940) & BIT(7));
}

void arm940Halt() {
  REG16(DUALCTRL940) |= BIT(7);
}

void arm940Run() {
  REG16(DUALCTRL940) &= ~BIT(7);
}

void arm940ClockOn() {
  REG16(SYSCLKENREG) |= BIT(0);
}

void arm940ClockOff() {
  REG16(SYSCLKENREG) &= ~BIT(0);
}

bool arm940HasClock() {
  return REG16(DUALCTRL940) & BIT(0);
}

extern void orcus_configure_peripherals();
// memoryBank can be 0-3, each representing that addresses on the 940 will be +(16M*memoryBank)
int arm940Init(int memoryBank) {
  if(memoryBank < 1 || memoryBank > 3) {
    return 1;
  }
  REG16(DUALCTRL940) = (REG16(DUALCTRL940) & 0xFF) | BIT(7) | (memoryBank & 0x7F);

  arm940ClockOn();
  REG16(DUALINT920) = 0x0;
  REG16(DUALINT940) = 0x0;

  return 0;
}

#pragma GCC optimize ("O0")
bool arm940IsThis() {
  return ((int)&__is_arm940) == 1;
}

void* arm9x0ImportPointer(uint32_t addr) {
  int bank = ((int)&__arm940_bank);
  if(arm940IsThis()) {
    return (void*) (addr - (bank * 0x1000000));
  } else {
    return (void*) (addr + (bank * 0x1000000));
  }
}
