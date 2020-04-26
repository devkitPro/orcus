#include <gp2xregs.h>
#include <orcus.h>

void nandRead(uint32_t startAddr, int numberOfBlocks, void* dest) {
  for(int i = numberOfBlocks ; i-- ; ) {
    REG_NFCMD = 0;
    REG_NFADDR = (startAddr&0xFF);
    REG_NFADDR = ((startAddr>>9)&0xFF);
    REG_NFADDR = ((startAddr>>17)&0xFF);
    REG_NFADDR = ((startAddr>>25)&0xFF);

    while(!(REG16(MEMNANDCTRLW) & 0x80));

    for(int j = 0 ; j < NAND_BLOCK_SIZE ; j+=2 ) {
      *((uint16_t*)dest) = REG_NFDATA;
    }
  }
}
