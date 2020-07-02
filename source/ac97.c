#include <gp2xregs.h>
#include <orcus.h>

#define SPDIF_CONFIG 0x2A
#define DACSR 0x2C
#define AUXDACSR 0x2E
#define ADCSR 0x32

void ac97Start() {
  REG16(AC_STA_ENA_REG) = BIT(7) | BIT(6) | BIT(5);
  REG16(AC_CTRL_REG) = BIT(2) | BIT(0);
  timerSleepNs(10000000);
  REG16(AC_CTRL_REG) = BIT(2);
}

uint16_t ac97GetReg(uint8_t reg) {
  while(REG16(AC_CODEC_ACCESS)); // wait until no codec access in progress
  
  volatile uint16_t out = REG16((0x0F00 | reg));
  while(!(REG16(AC_STATUS_REG) & BIT(5)));
  out = REG16(AC_CODEC_DATA);
  return out;
}

void ac97SetReg(uint8_t reg, uint16_t value) {
  while(REG16(AC_CODEC_ACCESS)); // wait until no codec access in progress
  
  REG16((0x0F00 | reg)) = value;
  while(!(REG16(AC_STATUS_REG) & BIT(6)));
}

void ac97SetSampleRate(uint16_t khz) {
  uint16_t spdifCfg = ac97GetReg(SPDIF_CONFIG);
  spdifCfg |= BIT(0); // VRA, allow rates other than 48kHz
  ac97SetReg(SPDIF_CONFIG, spdifCfg);

  ac97SetReg(DACSR, khz);
  ac97SetReg(AUXDACSR, khz);
  ac97SetReg(ADCSR, khz);
}
