#include <gp2xregs.h>
#include <orcus.h>
#include <stddef.h>
#include <unistd.h>
#include "disc_io.h"

extern void orcus_delay(int loops);

#define MMC_SPEED 10000000
#define SD_SPEED 20000000
#define INITIAL_SD_SPEED 400000

#define ILLEGAL_COMMAND BIT(22)

static uint16_t rca;
static int sizeKb = -1;
static bool isMMC = false;
static bool isSDHC = false;

int sdSizeKb() {
  return sdIsInserted() ? sizeKb : -1;
}

void sdSetClock(int hz) {
  REG16(SDIPRE) = (74649600 / hz) - 1;
  usleep(20000);
}

static int sd_cmd(uint8_t command, uint32_t arg, bool awaitResponse, bool isLongResponse, bool ignoreCrc) {
  REG16(SDICmdSta) = 0x0F00; // clear flags

  REG16(SDICmdArgL) = arg&0xFFFF;
  REG16(SDICmdArgH) = (arg&0xFFFF0000)>>16;

  REG16(SDICmdCon) = ((isLongResponse ? 1 : 0) << 10) | ((awaitResponse ? 1 : 0) << 9) | (1 << 8) | command | 0x40; // for some reason you have to set the upper two bits to 0b10

  volatile uint16_t statusReg = REG16(SDICmdSta);
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
    if(sd_cmd(55, 0, true, false, false)) {
      usleep(20000);
      continue;
    }
    if(!sd_cmd(41, 0xff8000 | (1 << 30), true, false, true)) { // we do support SDHC/SDXC
      if(REG16(SDIRSP1) & 0x8000) {
	return 0;
      } //  else card not ready
    }
    usleep(20000);
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
    isSDHC = true;
    return (REG16(SDIRSP4) + 1)*512;
  }  
}

uint32_t r1() {
  return REG16(SDIRSP1) << 16 | REG16(SDIRSP0);
}

uint32_t cmd13() {
  if(!sd_cmd(13, (rca << 16), true, false, false)) {
    return r1();
  }
  return 0xFFFFFFFF;
}


int sdInit() {
  isSDHC = false;
  isMMC = false;
  sdSetClock(INITIAL_SD_SPEED);
  REG16(SDIDatConL) = 0x4000; // make sure all Rx/Tx is halted before we go any further
  REG16(SDICON) = SDICON_BYT_ORDER | BIT(1) | SDICON_ENCLK(1);
  REG16(SDIDTimerL) = 0xFFFF;
  REG16(SDIDTimerH) = 0x001F;
  REG16(SDIBSize) = 512;
  orcus_delay(0x1000);

  for(int i = 100 ; i-- ; ) {
    if(!sd_cmd(0, 0, false, false, true)) {
      break;
    }

    if(i == 0) {
      return 1;
    }
  }

  if(sd_cmd(8, 0x000001AA, true, false, false)) {
    // could be MMC

    while(!sd_cmd(1, 0xffc000, true, false, true)) {
      if(r1() & 0x80000000) {
	isMMC = true;
	goto IS_READY;
      }
    }

    if(!sd_waitReady()) {
      // this is an SD1.x card
      goto IS_READY;
    }
    
    return 2;
  }
      
  if(sd_waitReady()) {
    return 3;
  }

 IS_READY:
  while(sd_cmd(2, 0, true, true, false));
  while(sd_cmd(3, 0, true, false, false));

  if(isMMC)
    rca = 0;
  else
    rca = REG16(SDIRSP1);

  if(sd_cmd(9, (rca << 16), true, true, false)) {
    return 4;
  }
  sizeKb = sd_calculateSizeKb();

  sdSetClock(isMMC ? MMC_SPEED : SD_SPEED);

  // select card
  if(sd_cmd(7, (rca << 16), true, false, false)) {
    return 5;
  }

  return 0;
}

int sdReadBlocks(int startBlock, int numberOfBlocks, uint8_t* dest) {
  int attempts = 0;

 CMD18:
  REG16(SDICmdSta) = 0xFFFF;
  REG16(SDIDatSta) = 0x07FF;
  REG16(SDIDatConL) = (2 << 12) | numberOfBlocks;
  REG16(SDIDatConH) = 0x000A;
  REG16(SDICON) |= BIT(1); // clear the FIFO before we try to read
  
  if(sd_cmd(18, startBlock*(isSDHC ? 1 : 512), true, false, false)) {
    if(cmd13()&0x400000) { // sometimes the card seems to get confused and stuck in data mode, reinit if we start getting illegal command responses
      sdInit();
    }
    if(attempts > 5) {
      return 1;
    } else {
      attempts++;
      usleep(20000);
      goto CMD18;
    }
  } else {
    REG16(SDICmdSta) = 0x1E00;

    for(int block = 0 ; block < numberOfBlocks ; block++) {
      for(int byte = 0 ; byte < 512 ; byte++) { // TODO - handle different block sizes
	uint32_t currentTimer = timerGet();
	while((REG16(SDIFSTA)&0x7F) == 0) {
	  if(timerNsSince(currentTimer, NULL) > 33750000) {
	    goto CMD18;
	  }
	}
	dest[(block*512)+byte] = REG8(SDIDAT);
      }
    }

    REG16(SDIDatConL) |= BIT(14);
    REG16(SDICON) |= BIT(1);
    
    if(sd_cmd(12, 0, true, false, false)) {
      return 2;
    }

    REG16(SDICON) |= BIT(1);

    REG16(SDIDatConL) = 0xFFFF;
    REG16(SDIDatConH) = 0xFFFF;
  }

  return 0;
}

int sdWriteBlocks(int startBlock, int numberOfBlocks, uint8_t* src) {
  int attempts = 0;

 CMD25:
  REG16(SDICmdSta) = 0xFFFF;
  REG16(SDIDatSta) = 0x07FF;
  REG16(SDIDatConL) = (3 << 12) | numberOfBlocks;
  REG16(SDIDatConH) = 0x0012;
  REG16(SDICON) |= BIT(1); // clear the FIFO before we try to write

  if(sd_cmd(25, startBlock*(isSDHC ? 1 : 512), true, false, false)) {
    if(cmd13()&0x400000) { // sometimes the card seems to get confused and stuck in data mode, reinit if we start getting illegal command responses
      sdInit();
    }
    if(attempts > 5) {
      return 1;
    } else {
      attempts++;
      usleep(20000);
      goto CMD25;
    }
  } else {
    REG16(SDICmdSta) = 0x1E00;

    for(int block = 0 ; block < numberOfBlocks ; block++) {
      for(int byte = 0 ; byte < 512 ; byte++) { // TODO - handle different block sizes
	uint32_t currentTimer = timerGet();
	while((REG16(SDIFSTA)&0x2000) == 0) {
	  if(timerNsSince(currentTimer, NULL) > 33750000) {
	    goto CMD25;
	  }
	}
	REG8(SDIDAT) = src[(block*512)+byte];
      }
    }

    REG16(SDIDatConL) |= BIT(14);
    REG16(SDICON) |= BIT(1);

    if(sd_cmd(12, 0, true, false, false)) {
      return 2;
    }

    REG16(SDICON) |= BIT(1);

    REG16(SDIDatConL) = 0xFFFF;
    REG16(SDIDatConH) = 0xFFFF;
  }

  return 0;
}

bool sd_Startup() {
  return sdIsInserted() && !sdInit();
}

bool sdIsInserted() {
  return (REG16(GPIOIPINLVL) & 0x4000) == 0;
}

bool sd_ReadSectors(sec_t sector, sec_t numSectors, void* buffer) {
  return !sdReadBlocks(sector, numSectors, (uint8_t*) buffer);
}

bool sd_WriteSectors(sec_t sector, sec_t numSectors, const void* buffer) {
  return !sdWriteBlocks(sector, numSectors, (uint8_t*) buffer);
}

bool sd_ClearStatus() {
  return true;
}

bool sd_Shutdown() {
  return true;
}

#define DEVICE_TYPE_GP2X_SD ('_') | ('S' << 8) | ('D' << 16) | ('_' << 24)

const DISC_INTERFACE __io_gp2xsd = {
	DEVICE_TYPE_GP2X_SD,
	FEATURE_MEDIUM_CANREAD | FEATURE_MEDIUM_CANWRITE,
	(FN_MEDIUM_STARTUP)&sd_Startup,
	(FN_MEDIUM_ISINSERTED)&sdIsInserted,
	(FN_MEDIUM_READSECTORS)&sd_ReadSectors,
	(FN_MEDIUM_WRITESECTORS)&sd_WriteSectors,
	(FN_MEDIUM_CLEARSTATUS)&sd_ClearStatus,
	(FN_MEDIUM_SHUTDOWN)&sd_Shutdown
};

const DISC_INTERFACE* get_io_gp2xsd (void) {
  return &__io_gp2xsd;
}

// initialization resources:
//http://www.rjhcoding.com/avrc-sd-interface-2.php
// https://yannik520.github.io/sdio.html#sec-3-6-3
//https://www.microchip.com/forums/m191802.aspx

// libfat... https://github.com/devkitPro/libnds/blob/master/source/arm9/dldi/sd.twl.c
// https://github.com/devkitPro/libfat/blob/master/source/disc.c
