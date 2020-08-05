#include <stdint.h>
#include <stdlib.h>
#include <gp2xregs.h>
#include <orcus.h>

/*
  Useful documentation:

  - Turning caches/MMU on/off... https://developer.arm.com/documentation/ddi0151/c/programmer-s-model/cp15-register-map-summary/register-1--control-register?lang=en
  - Cache control... // https://developer.arm.com/documentation/ddi0151/c/programmer-s-model/cp15-register-map-summary/register-7--cache-operations-register
  - Descriptors... https://developer.arm.com/documentation/ddi0151/c/memory-management-unit/address-translation/section-descriptor
  - TTB... https://developer.arm.com/documentation/ddi0151/c/programmer-s-model/cp15-register-map-summary/register-2--translation-table-base--ttb--register
*/

// 

// https://developer.arm.com/documentation/ddi0151/c/programmer-s-model/cp15-register-map-summary/register-7--cache-operations-register
uint32_t* mmuNewL1Table() {
  uint32_t* l1Table = NULL;
  // allocate a table of 4096 x 32-bit entries aligned to a 16K boundary
  if(posix_memalign((void**)&l1Table, MMU_L1_ALIGN, 0x4000) == 0) {
    for(int i = 4096 ; i-- ; ) {
      l1Table[i] = i < 64 ? SECTION_DESCRIPTOR(i*SZ_1M, AP(READ_WRITE, true), 0, true, true)
      	: SECTION_DESCRIPTOR(i*SZ_1M, AP(READ_WRITE, true), 0, false, false);
    }
    return l1Table;
  } else {
    return NULL;
  }
}

uint32_t mmuSetDomainAccess(unsigned int domain, DomainAccess access) {
  uint32_t domainAccess;
  uint32_t mask = (0x3 << (2*domain));
  uint32_t accessBits = (access << (2*domain));

  asm volatile("mrc p15, 0, %0, c3, c0, 0;  \
                and %0, %0, %1;  \
                orr %0, %0, %2;  \
                mcr p15, 0, %0, c3, c0, 0"
	       :"=r"(domainAccess)
	       :"r"(mask), "r"(accessBits)
	       );

  return domainAccess;
}

// https://developer.arm.com/documentation/ddi0151/c/programmer-s-model/cp15-register-map-summary/register-1--control-register?lang=en
void enableICache() {
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;  \
                orr r0, r0, #(0x1 << 12);  \
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}

void disableICache() {
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;  \
                and r0, r0, #0xFFFFEFFF;  \
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}

// NOTE! This doesn't write-back dirty data
void invalidateICache() {
  asm volatile("mov r0, #0;  \
                mcr p15, 0, r0, c7, c5, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );	       
}


void enableDCache() {
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;  \
                orr r0, r0, #0x5;  \
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}

void disableDCache() {
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;  \
                and r0, r0, #0xFFFFFFFB;  \
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}


// NOTE! This doesn't write-back dirty data
void invalidateDCache() {
  asm volatile("mov r0, #0;  \
                mcr p15, 0, r0, c7, c6, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );	       
}

// also disables dcache
void mmuEnable(void* l1Table) {
  
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;	\
                orr r0, r0, #0x1;		\
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}

void mmuDisable() {
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;  \
                and r0, r0, #0xFFFFFFFA;  \
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}

void mmuCachesInitOn() {
  
}
