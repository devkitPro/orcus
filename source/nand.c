#include <gp2xregs.h>
#include <orcus.h>

void nandRead(uint32_t startAddr, int numberOfBlocks, void* dest) {
  for(int i = numberOfBlocks ; i-- ; ) {
    NANDREG8(NFCMD) = 0;
    NANDREG8(NFADDR) = (startAddr&0xFF);
    NANDREG8(NFADDR) = ((startAddr>>9)&0xFF);
    NANDREG8(NFADDR) = ((startAddr>>17)&0xFF);
    NANDREG8(NFADDR) = ((startAddr>>25)&0xFF);

    while(!(REG16(MEMNANDCTRLW) & 0x80));

    for(int j = 0 ; j < NAND_BLOCK_SIZE ; j+=2 ) {
      *((uint16_t*)dest) = NANDREG16(NFDATA);
    }
  }
}
