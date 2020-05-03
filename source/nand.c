#include <unistd.h>
#include <gp2xregs.h>
#include <orcus.h>

void nandRead(uint32_t startAddr, int numberOfBlocks, void* dest) {
  uint16_t* d = (uint16_t*) dest;
  uint32_t addr = startAddr;
  for(int i = numberOfBlocks ; i-- ; ) {
    NANDREG8(NFCMD) = 0;
    NANDREG8(NFADDR) = (addr&0xFF);
    NANDREG8(NFADDR) = ((addr>>9)&0xFF);
    NANDREG8(NFADDR) = ((addr>>17)&0xFF);
    NANDREG8(NFADDR) = ((addr>>25)&0xFF);

    while(!(REG16(MEMNANDCTRLW) & 0x8000));
    REG16(MEMNANDCTRLW) = 0x8080;
    
    for(int j = 0 ; j < NAND_BLOCK_SIZE ; j+=2 ) {
      *(d++) = NANDREG16(NFDATA);
    }
    addr += NAND_BLOCK_SIZE;
  }
}
