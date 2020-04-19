#include <gp2xregs.h>
#include <orcus.h>

#define MMC_SPEED 10000000
#define SD_SPEED 25000000
#define INITIAL_SD_SPEED 100000

void sdSetClock(int hz) {
  REG16(SDIPRE) = (74649600 / hz) - 1;
  orcus_delay(0x1000);
}

static int sd_cmd(uint8_t command, uint32_t arg, bool awaitResponse, bool isLongResponse, bool ignoreCrc) {
  REG16(SDICmdSta) = 0x0F00; // clear flags

  REG16(SDICmdArgL) = arg&0xFFFF;
  REG16(SDICmdArgH) = (arg&0xFFFF0000)>>16;

  uint16_t commandReg = ((isLongResponse ? 1 : 0) << 10) | ((awaitResponse ? 1 : 0) << 9) | (1 << 8) | command | 0x40; // for some reason you have to set the upper two bits to 0b10
  REG16(SDICmdCon) = commandReg;

  uint16_t statusReg = REG16(SDICmdSta);
  // wait for command to finish
  if(awaitResponse) {
    while(!(statusReg & 0x200 || statusReg & 0x400)) {
      statusReg = REG16(SDICmdSta);
    }
  } else {
    while(!(statusReg & 0x800)) {
      statusReg = REG16(SDICmdSta);
    }
  }

  if(statusReg & 0x400) return 1; // timeout
  else if(!ignoreCrc && (statusReg & 0x1000)) return 2; // crc error
  else return 0;
}

static int sd_waitReady() {
  // apparently you have to repeat this sequence many times for some cards, u-boot does it 150 times, wait  for card to be ready
  for(int i = 0 ; i < 150 ; i++) {
    if(sd_cmd(55, 0, true, false, false)) { continue; }
    if(!sd_cmd(41, 0xff8000 | (1 << 30), true, false, true)) { // we do support SDHC/SDXC
      if(REG16(SDIRSP1) & 0x8000) {
	return 0;
      } //  else card not ready
    }
  }
  return 1;
}

static int sd_calculateSizeKb() {
  uint8_t csdStructure = (REG16(SDIRSP1) & 0xC000) >> 14;
  if(csdStructure > 1) { return -1; } // invalid CSD structure number

  if(csdStructure == 0) {
    int readBlLen = 9;// REG16(SDIRSP2) & 0xF;
    int cSize = ((REG16(SDIRSP3) & 0x3FF) << 2) | ((REG16(SDIRSP4) & 0xC000) >> 14);
    int cSizeMult = (REG16(SDIRSP5) & 0xE000) >> 13;
    
    int mult = 1 << (cSizeMult+2);
    int blockLen = 1 << readBlLen;
    int blockNr = (cSize)+1 * mult;
    return blockNr * blockLen; // TODO - I think this is wrong
  } else {
    return (REG16(SDIRSP4) + 1)*512;
  }  
}


void sdInit(SdInfo* info) {
  info->isInserted = false;
  info->sizeKb = -1;
  info->rca = -1;
  
  sdSetClock(INITIAL_SD_SPEED);
  REG16(SDICON) = SDICON_BYT_ORDER | SDICON_ENCLK(1);
  REG16(SDIDTimerL) = 0xFFFF;
  REG16(SDIDTimerH) = 0x001F;
  REG16(SDIBSize) = 512;
  orcus_delay(0x1000);
    
  if(sd_cmd(0, 0, false, false, true)) {
    uart_printf("CMD0 failed\r\n");
    return;
  }

  if(sd_cmd(8, 0x000001AA, true, false, false)) {
    uart_printf("CMD8 failed\r\n");
    return;
  }
      
  if(sd_waitReady()) {
    uart_printf("SD card never became ready\r\n");
    return;
  }

  while(sd_cmd(2, 0, true, true, false));
  while(sd_cmd(3, 0, true, false, false));

  uint16_t rca = REG16(SDIRSP1);

  if(sd_cmd(9, (rca << 16), true, true, false)) {
    return;
  }
  int sizeKb = sd_calculateSizeKb();

  // select card
  if(sd_cmd(7, (rca << 16), true, false, false)) {
    return;
  }

  // set bus to full width SD
  /*  while(true) {
  if(sd_cmd(55, 0, true, false, false)) {
    continue;
  }
  if(!sd_cmd(6, 1, true, false, false)) {
    break;
  }
  }*/
  sdSetClock(SD_SPEED);

  info->isInserted = true;
  info->sizeKb = sizeKb;
  info->rca = rca;
}

void sdReadBlocks(int startBlock, int numberOfBlocks, uint8_t* dest) {
  REG16(SDICmdSta) = 0x0200;
  REG16(SDIDatSta) = 0x07FF;
  REG16(SDIDatConL) = (2 << 12) | numberOfBlocks;
  REG16(SDIDatConH) = 0x000A;
  // TODO - if not sdhc, startBlock*512, rather than just startBlock as argument)
  int response = sd_cmd(18, startBlock, true, false, false);
  if(response) {
    uart_printf("couldn't start reading\r\n");
  } else {
    REG16(SDICmdSta) = 0x1E00;

    for(int block = 0 ; block < numberOfBlocks ; block++) {
      for(int byte = 0 ; byte < 512 ; byte++) { // TODO - handle different block sizes
	while(!(REG16(SDIFSTA) & (1 << 12))) { // TODO - we should handle errors and break out what would otherwise end up an infinite loop
	  uart_printf("waiting for data to become available\r\n");
	}
	dest[(block*512)+byte] = REG8(SDIDAT);
      }
    }

    uart_printf("read blocks\r\n");
    while(!(REG16(SDIDatSta) & (1 << 4)));
    REG16(SDIDatConL) |= (1 << 14);
    if(sd_cmd(12, 0, true, false, false)) {
      uart_printf("Couldn't stop transmission\r\n");
    }
    
    // TODO - finish this, page 572 of MMSP2 databook
  }
  
}

void orcus_configure_sd() {
  //  SDICON = 0x11 (10001)
    //    prescaler = 4 for MMC, 3 for SD, pclk is 74649600
  // wait for 74 SD clock cycles
  // (SDIBSize) = 512
  // (SDIDatConH) = 0b1011
  // when sdicmdcon says CMD_INDEX Command index with start 2bit(8bit) it means prefix 2 to the command, i.e. the upper two bits are always 0b01 - who knows why?
  // handy website for commands http://www.chlazza.net/sdcardinfo.html
  // sdhc takes addresses as blocks, sd as byte addresses. Should always read aligned to 512 byte block bounary

  // initialization resources:
  //http://www.rjhcoding.com/avrc-sd-interface-2.php
  // https://yannik520.github.io/sdio.html#sec-3-6-3
  //https://www.microchip.com/forums/m191802.aspx

  // libfat... https://github.com/devkitPro/libnds/blob/master/source/arm9/dldi/sd.twl.c
  // https://github.com/devkitPro/libfat/blob/master/source/disc.c

  //          rSDICON = rSDICON | (1<<1);                     // FIFO reset
  //       rSDIBSIZE  = 0x200;                                     // 512byte(128word)
        /* Set timeout count */
  ///     rSDIDTIMERL = 0xffff;
  ///    rSDIDTIMERH = 0x001f;

}