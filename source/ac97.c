#include <gp2xregs.h>
#include <orcus.h>

#define SPDIF_CONFIG 0x2A
#define DACSR 0x2C
#define AUXDACSR 0x2E
#define ADCSR 0x32
#define DACS 0x18

#define SPKOUT 0x02
#define HPOUT 0x04

#define AUDIO_BASE 0xC0000E00

static int audioDmaChannel = 0;

void ac97Start() {
  REG16(AC_CTRL_REG) = BIT(2) | BIT(0);
  REG16(AC_STA_ENA_REG) = BIT(7) | BIT(6) | BIT(5);
  timerSleepNs(10000000);
  REG16(AC_CTRL_REG) = BIT(2);
  while(!(REG16(AC_STATUS_REG) & BIT(7)));
}

void ac97Stop() {
  REG16(AC_CTRL_REG) = 0x1;
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

void audioSetSampleRate(uint16_t khz) {
  uint16_t spdifCfg = ac97GetReg(SPDIF_CONFIG);
  spdifCfg |= BIT(0); // VRA, allow rates other than 48kHz
  ac97SetReg(SPDIF_CONFIG, spdifCfg);

  ac97SetReg(DACSR, khz);
  ac97SetReg(AUXDACSR, khz);
  ac97SetReg(ADCSR, khz);
}

void audioMuteSpeakers() {
  uint16_t out = ac97GetReg(SPKOUT);
  out |= BIT(15);
  ac97SetReg(SPKOUT, out);
}

void audioUnmuteSpeakers() {
  uint16_t out = ac97GetReg(SPKOUT);
  out &= ~BIT(15);
  ac97SetReg(SPKOUT, out);
}

void audioMuteHeadphones() {
  uint16_t out = ac97GetReg(HPOUT);
  out |= BIT(15);
  uartPrintf("Blah: 0x%x\n", out);
  ac97SetReg(HPOUT, out);
}

void audioUnmuteHeadphones() {
  uint16_t out = ac97GetReg(HPOUT);
  out &= ~BIT(15);
  ac97SetReg(HPOUT, out);
}

void audioInit(int dmaChannel) {
  audioDmaChannel = dmaChannel;
  dmaConfigureChannelIO(dmaChannel, WORDS_4, 1, 0, AC97_LRPCM);
  ac97Start();

  uint16_t dacs = ac97GetReg(DACS);
  ac97SetReg(DACS, dacs & 0x1FFF);
}

void audioSetVolume(uint8_t left, uint8_t right) {
  uint8_t leftVol = left > 31 ? 31 : ((~left)&0x1f);
  uint8_t rightVol = right > 31 ? 31 : ((~right)&0x1f);

  uint16_t spkReg = ac97GetReg(SPKOUT);
  spkReg = (spkReg & 0xC0FF) | (leftVol << 8);
  spkReg = (spkReg & 0xFFC0) | rightVol;
  ac97SetReg(SPKOUT, spkReg);
  
  uint16_t hpReg = ac97GetReg(HPOUT);
  hpReg = (hpReg & 0xC0FF) | (leftVol << 8);
  hpReg = (hpReg & 0xFFC0) | rightVol;
  ac97SetReg(HPOUT, hpReg);
}

void audioPlaySample(uint16_t bytes, void* data) {
  dmaStart(audioDmaChannel, bytes, (uint32_t)data, AUDIO_BASE);
}

bool audioSamplePlaying() {
  return dmaIsTransferring(audioDmaChannel);
}

bool audioHeadphonesConnected() {
  // TODO
  return false;
}
