#include <gp2xregs.h>
#include <orcus.h>

void dmaConfigureChannelMem(int channel, BurstMode burstMode, int8_t srcIncrement, int8_t destIncrement) {
  REG16(DCH0SRM + (channel * 4)) &= 0xFF80; // this is not an IO device
  REG16(DMAREG(DMACOM0, channel)) = (burstMode << 14)
    | ((srcIncrement == 0 ? 0x0 : 0x1) << 13)
    | ((destIncrement == 0 ? 0x0 : 0x1) << 5);
  REG16(DMAREG(DMACOM1, channel)) = (srcIncrement << 8) | destIncrement;
  REG16(DMAREG(DMACONS, channel)) = 0x0; // TODO examine fly by mode
  
}

void dmaConfigureChannelIO(int channel, BurstMode burstMode, int8_t srcIncrement, int8_t destIncrement, Peripheral peripheral) {
  REG16(DCH0SRM + (channel * 4)) = BIT(6) | peripheral;
  REG16(DMAREG(DMACOM0, channel)) = (burstMode << 14)
    | ((srcIncrement == 0 ? 0x0 : 0x1) << 13)
    | ((destIncrement == 0 ? 0x0 : 0x1) << 5)
    | BIT(4)
    | BIT(1);
  REG16(DMAREG(DMACOM1, channel)) = (srcIncrement << 8) | destIncrement;
  REG16(DMAREG(DMACONS, channel)) = 0x0; // TODO examine fly by mode

}

void dmaStart(int channel, uint16_t length, uint32_t src, uint32_t dest) {
  REG16(DMAREG(DMACONS, channel)) &= ~(BIT(1) | BIT(2));
  REG32(DMAREG(DMASRCADDR, channel)) = src;
  REG32(DMAREG(DMATRGADDR, channel)) = dest;
  REG16(DMAREG(DMACOM2, channel)) = length;
  REG16(DMAREG(DMACONS, channel)) |= BIT(10);
}

void dmaStop(int channel) {
  REG16(DMAREG(DMACONS, channel)) &= ~BIT(10);
}

bool dmaIsTransferring(int channel) {
  return REG16(DMAREG(DMACONS, channel)) & BIT(2);
}

bool dmaHasFinished(int channel) {
  return REG16(DMAREG(DMACONS, channel)) & BIT(1);  
}
