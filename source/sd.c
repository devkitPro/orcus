#include <gp2xregs.h>
#include <orcus.h>

void orcus_configure_sd() {
  //  SDICON = 0x11 (10001)
    //    prescaler = 4 for MMC, 3 for SD, pclk is 74649600
  // wait for 74 SD clock cycles
  // (SDIBSize) = 512
  // (SDIDatConH) = 0b1011
  // when sdicmdcon says CMD_INDEX Command index with start 2bit(8bit) it means prefix 2 to the command, i.e. the upper two bits are always 0b01 - who knows why?
  // handy website for commands http://www.chlazza.net/sdcardinfo.html
  // sdhc takes addresses as blocks, sd as byte addresses. Should always read aligned to 512 byte block bounary

  // libfat... https://github.com/devkitPro/libnds/blob/master/source/arm9/dldi/sd.twl.c
  // https://github.com/devkitPro/libfat/blob/master/source/disc.c
}
