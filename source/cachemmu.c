#define _GNU_SOURCE

#include <stdint.h>
#include <malloc.h>
#include <gp2xregs.h>
#include <orcus.h>

/*
  Useful documentation:

  - Turning caches/MMU on/off... https://developer.arm.com/documentation/ddi0151/c/programmer-s-model/cp15-register-map-summary/register-1--control-register?lang=en
  - Cache control... // https://developer.arm.com/documentation/ddi0151/c/programmer-s-model/cp15-register-map-summary/register-7--cache-operations-register
  - Descriptors... https://developer.arm.com/documentation/ddi0151/c/memory-management-unit/address-translation/section-descriptor
  - TTB... https://developer.arm.com/documentation/ddi0151/c/programmer-s-model/cp15-register-map-summary/register-2--translation-table-base--ttb--register
  - PU on 940... https://developer.arm.com/documentation/ddi0092/b/protection-unit?lang=en
*/

void cacheEnableI() {
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;  \
                orr r0, r0, #(0x1 << 12);  \
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}

void cacheDisableI() {
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;  \
                bic r0, r0, #(0x1 << 12); \
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}

// NOTE! This doesn't write-back dirty data
void cacheInvalidateI() {
  asm volatile("mov r0, #0;  \
                mcr p15, 0, r0, c7, c5, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );	       
}

void cacheEnableD() {
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;  \
                orr r0, r0, #0x5;  \
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}

void cacheDisableD() {
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;  \
                bic r0, r0, #(0x1 << 2); \
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}


// NOTE! This doesn't write-back dirty data
void cacheInvalidateD() {
  asm volatile("mov r0, #0;  \
                mcr p15, 0, r0, c7, c6, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );	       
}

void cacheCleanD() {
  bool isArm940 = arm940IsThis();

  if(!isArm940) {
    for(uint32_t segment = 0 ; segment < 8 ; segment++) {
      for(uint32_t index = 0 ; index < 64 ; index++) {
	uint32_t r = (index << 26) | (segment << 5);
	asm volatile("mcr p15, 0, %[r], c7, c10, 2"
		     : // no outputs
		     : [r] "r" (r)
		     );
      }
    }
  } else {
    for(uint32_t segment = 0 ; segment < 8 ; segment++) {
      for(uint32_t index = 0 ; index < 64 ; index++) {
	uint32_t r = (index << 26) | (segment << 5);
	asm volatile("mcr p15, 0, %[r], c7, c10, 1"
		     : // no outputs
		     : [r] "r" (r)
		     );
      }
    }
  }
}

void cacheInvalidateDI() {
  bool isArm940 = arm940IsThis();
  uint32_t r = 0x0;
  
  if(!isArm940) {
    asm volatile("mcr p15, 0, %[r], c7, c7, 0"
		 : // no outputs
		 :[r] "r" (r)
		 );
  } else {
    asm volatile("mcr p15, 0, %[r], c7, c5, 0; \
		  mcr p15, 0, %[r], c7, c6, 0"
		 : // no outputs
		 :[r] "r" (r));
  }
}

void mmuEnable(void* l1Table) {
  asm volatile("mcr p15, 0, %0, c2, c0, 0;  \
                mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;	\
                orr r0, r0, #0x1;		\
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       :"r"(l1Table)
	       :"r0"
	       );
}

// also disables dcache
void mmuDisable() {
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;  \
                bic r0, r0, #0x5;  \
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}

uint32_t* mmuNewL1Table() {
  uint32_t* l1Table = (uint32_t*) memalign(MMU_L1_ALIGN, 0x4000);
  // allocate a table of 4096 x 32-bit entries aligned to a 16K boundary
  if(l1Table != NULL) {
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
  uint32_t domainAccess = 0;
  uint32_t mask = (0x3 << (2*domain));
  uint32_t accessBits = (access << (2*domain));

  asm volatile("mrc p15, 0, %0, c3, c0, 0;  \
                and %0, %0, %1;  \
                orr %0, %0, %2;  \
                mcr p15, 0, %0, c3, c0, 0"
	       :"+r"(domainAccess)
	       :"r"(mask), "r"(accessBits)
	       :"cc"
	       );

  return domainAccess;
}

void mmuCachesInitOn() {
  cacheInvalidateDI();
  cacheEnableI();
  mmuSetDomainAccess(0, MANAGER);
  mmuEnable(mmuNewL1Table());
  cacheEnableD();
}

void puSetDRegion(unsigned int region, uint32_t params, PUAccess access, bool cacheable, bool buffered) {
  uint32_t cacheableFlag = cacheable ? (0x1 << region) : 0;
  uint32_t bufferedFlag = buffered ? (0x1 << region) : 0;
  uint32_t accessFlag = access << (2*region);
  uint32_t tmp = 0;
  
  switch(region) {
  case 0:
    asm volatile("mcr p15, 0, %[params], c6, c0, 0; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 0; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*0)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 0; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 0); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c3, c0, 0; /* Update buffered register */ \
                  bic %[tmp], %[tmp], #(0x1 << 0); \
                  orr %[tmp], %[tmp], %[bufferedFlag]; \
                  mcr p15, 0, %[tmp], c3, c0, 0;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [bufferedFlag] "r" (bufferedFlag),
		   [tmp] "r" (tmp));
    break;
  case 1:
    asm volatile("mcr p15, 0, %[params], c6, c1, 0; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 0; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*1)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 0; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 1); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c3, c0, 0; /* Update buffered register */ \
                  bic %[tmp], %[tmp], #(0x1 << 1); \
                  orr %[tmp], %[tmp], %[bufferedFlag]; \
                  mcr p15, 0, %[tmp], c3, c0, 0;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [bufferedFlag] "r" (bufferedFlag),
		   [tmp] "r" (tmp));
    break;
  case 2:
    asm volatile("mcr p15, 0, %[params], c6, c2, 0; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 0; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*2)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 0; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 2); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c3, c0, 0; /* Update buffered register */ \
                  bic %[tmp], %[tmp], #(0x1 << 2); \
                  orr %[tmp], %[tmp], %[bufferedFlag]; \
                  mcr p15, 0, %[tmp], c3, c0, 0;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [bufferedFlag] "r" (bufferedFlag),
		   [tmp] "r" (tmp));
    break;
  case 3:
    asm volatile("mcr p15, 0, %[params], c6, c3, 0; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 0; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*3)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 0; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 3); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c3, c0, 0; /* Update buffered register */ \
                  bic %[tmp], %[tmp], #(0x1 << 3); \
                  orr %[tmp], %[tmp], %[bufferedFlag]; \
                  mcr p15, 0, %[tmp], c3, c0, 0;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [bufferedFlag] "r" (bufferedFlag),
		   [tmp] "r" (tmp));
    break;
  case 4:
    asm volatile("mcr p15, 0, %[params], c6, c4, 0; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 0; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*4)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 0; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 4); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c3, c0, 0; /* Update buffered register */ \
                  bic %[tmp], %[tmp], #(0x1 << 4); \
                  orr %[tmp], %[tmp], %[bufferedFlag]; \
                  mcr p15, 0, %[tmp], c3, c0, 0;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [bufferedFlag] "r" (bufferedFlag),
		   [tmp] "r" (tmp));
    break;
  case 5:
    asm volatile("mcr p15, 0, %[params], c6, c5, 0; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 0; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*5)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 0; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 5); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c3, c0, 0; /* Update buffered register */ \
                  bic %[tmp], %[tmp], #(0x1 << 5); \
                  orr %[tmp], %[tmp], %[bufferedFlag]; \
                  mcr p15, 0, %[tmp], c3, c0, 0;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [bufferedFlag] "r" (bufferedFlag),
		   [tmp] "r" (tmp));
    break;
  case 6:
    asm volatile("mcr p15, 0, %[params], c6, c6, 0; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 0; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*6)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 0; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 6); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c3, c0, 0; /* Update buffered register */ \
                  bic %[tmp], %[tmp], #(0x1 << 6); \
                  orr %[tmp], %[tmp], %[bufferedFlag]; \
                  mcr p15, 0, %[tmp], c3, c0, 0;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [bufferedFlag] "r" (bufferedFlag),
		   [tmp] "r" (tmp));
    break;
  case 7:
    asm volatile("mcr p15, 0, %[params], c6, c7, 0; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 0; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*7)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 0; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 7); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 0;     \
						     \
                  mrc p15, 0, %[tmp], c3, c0, 0; /* Update buffered register */ \
                  bic %[tmp], %[tmp], #(0x1 << 7); \
                  orr %[tmp], %[tmp], %[bufferedFlag]; \
                  mcr p15, 0, %[tmp], c3, c0, 0;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [bufferedFlag] "r" (bufferedFlag),
		   [tmp] "r" (tmp));
    break;    
  }
}

void puSetIRegion(unsigned int region, uint32_t params, PUAccess access, bool cacheable) {
  uint32_t cacheableFlag = cacheable ? (0x1 << region) : 0;
  uint32_t accessFlag = access << (2*region);
  uint32_t tmp = 0;
  
  switch(region) {
  case 0:
    asm volatile("mcr p15, 0, %[params], c6, c0, 1; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 1; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*0)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 1;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 1; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 0); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 1;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [tmp] "r" (tmp));
    break;
  case 1:
    asm volatile("mcr p15, 0, %[params], c6, c1, 1; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 1; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*1)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 1;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 1; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 1); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 1;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [tmp] "r" (tmp));
    break;
  case 2:
    asm volatile("mcr p15, 0, %[params], c6, c2, 1; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 1; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*2)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 1;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 1; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 2); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 1;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [tmp] "r" (tmp));
    break;
  case 3:
    asm volatile("mcr p15, 0, %[params], c6, c3, 1; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 1; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*3)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 1;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 1; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 3); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 1;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [tmp] "r" (tmp));
    break;
  case 4:
    asm volatile("mcr p15, 0, %[params], c6, c4, 1; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 1; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*4)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 1;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 1; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 4); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 1;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [tmp] "r" (tmp));
    break;
  case 5:
    asm volatile("mcr p15, 0, %[params], c6, c5, 1; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 1; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*5)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 1;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 1; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 5); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 1;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [tmp] "r" (tmp));
    break;
  case 6:
    asm volatile("mcr p15, 0, %[params], c6, c6, 1; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 1; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*6)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 1;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 1; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 6); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 1;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [tmp] "r" (tmp));
    break;
  case 7:
    asm volatile("mcr p15, 0, %[params], c6, c7, 1; /* Configure the region */ \
									\
                  mrc p15, 0, %[tmp], c5, c0, 1; /* Update access register */ \
                  bic %[tmp], %[tmp], #(0x3 << (2*7)); \
                  orr %[tmp], %[tmp], %[accessFlag]; \
                  mcr p15, 0, %[tmp], c5, c0, 1;     \
						     \
                  mrc p15, 0, %[tmp], c2, c0, 1; /* Update cacheable register */ \
                  bic %[tmp], %[tmp], #(0x1 << 7); \
                  orr %[tmp], %[tmp], %[cacheableFlag]; \
                  mcr p15, 0, %[tmp], c2, c0, 1;"
		 :
		 : [params] "r" (params),
		   [accessFlag] "r" (accessFlag),
		   [cacheableFlag] "r" (cacheableFlag),
		   [tmp] "r" (tmp));
    break;    
  }
}

void puEnable() {
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;  \
                orr r0, r0, #0x1;  \
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}

// also disables dcache
void puDisable() {
  asm volatile("mov r0, #0;  \
	        mrc p15, 0, r0, c1, c0, 0;  \
                bic r0, r0, #0x5;  \
	        mcr p15, 0, r0, c1, c0, 0"
	       : // no outputs
	       : // no inputs
	       :"r0"
	       );
}

void puCachesInitOn() {
  cacheInvalidateDI();
  cacheEnableI();
  puSetDRegion(0, PU_REGION(0x0, PU_64M, true), PU_FULL_ACCESS, true, true);
  puSetIRegion(0, PU_REGION(0x0, PU_64M, true), PU_FULL_ACCESS, true);
  puEnable();
  cacheEnableD();
}
