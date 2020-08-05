/*! \file cachemmu.h
    \brief Caches, MMU and PU
 */

#ifndef _MMU_H__
#define __MMU_H__

#include <stdint.h>

/** Alignment for L1 MMU table */
#define MMU_L1_ALIGN 0x4000
/** Alignment for L2 coarse table */
#define MMU_COARSE_ALIGN 0x400
/** Alignment for L2 fine table */
#define MMU_FINE_ALIGN 0x2000

#define SZ_1M 0x100000

typedef enum {
	      NO_ACCESS = 0x0,
	      CLIENT = 0x1, // use permission bits in translation tables
	      MANAGER = 0x3 // ignore permission bits
} DomainAccess;

typedef enum {
	      READ_WRITE = 0x0,
	      READ_ONLY = 0x2
} Access;

typedef enum {
	      LARGE_64K = 0x1,
	      SMALL_4K = 0x2,
	      TINY_1K = 0x3
} PageSize;

#define AP(access, el0Permitted) (access | (el0Permitted ? 1 : 0))

// el0 is unprivileged execution mode
// 1mb pages
#define SECTION_DESCRIPTOR(physicalAddr, ap, domain, cachable, buffered) (((physicalAddr) & 0xFFF00000) | (ap << 10) | (domain << 5) | BIT(4) | ((cachable ? 1 : 0) << 3) | ((buffered ? 1 : 0) << 2) | 0x2)

// 256 entries for large (64k) or small (4k) pages
#define COARSE_DESCRIPTOR(tablePhysicalAddr, domain) ((tablePhysicalAddr & 0xFFFFFC00) | (domain << 5) | BIT(4) | 0x1)

// 1024 entries for large (64k), small (4k) or tiny pages (1k)
#define FINE_DESCRIPTOR(tablePhysicalAddr, domain) ((tablePhysicalAddr & 0xFFFFF000) | (domain << 5) | BIT(4) | 0x3)

// subpages are 16k in size for ap
#define LARGE_DESCRIPTOR(physicalAddr, ap3, ap2, ap1, ap0, cachable, buffered) ((physicalAddr & 0xFFFF0000) | (ap3 << 10) | (ap2 << 8) | (ap1 << 6) | (ap0 << 4) | ((cachable ? 1 : 0) << 3) | ((buffered ? 1 : 0) << 2) | 0x1)
#define SMALL_DESCRIPTOR(physicalAddr, ap3, ap2, ap1, ap0, cachable, buffered) ((physicalAddr & 0xFFFFF000) | (ap3 << 10) | (ap2 << 8) | (ap1 << 6) | (ap0 << 4) | ((cachable ? 1 : 0) << 3) | ((buffered ? 1 : 0) << 2) | 0x2)
#define TINY_DESCRIPTOR(physicalAddr, ap3, ap2, ap1, ap0, cachable, buffered) ((physicalAddr & 0xFFFFFC00) | (ap3 << 10) | (ap2 << 8) | (ap1 << 6) | (ap0 << 4) | ((cachable ? 1 : 0) << 3) | ((buffered ? 1 : 0) << 2) | 0x3)

/**
   @brief Configures MMU domain access.

   Configures MMU domain access.

   @param domain Domain to configure (0 - 15)
   @param access Access permissions to assign to domain
   @return State of Domain Access Control Register after the change
 */
extern uint32_t mmuSetDomainAccess(unsigned int domain, DomainAccess access);

/**
   @brief Allocates and populates a new L1 MMU table.

   Allocates and populates a new L1 MMU table. This will by default enable caching and buffering for the first 64M 
   of the address space (i.e. RAM) using 1M section descriptors set to domain 0.
 */
extern uint32_t* mmuNewL1Table();

/**
   @brief Enables the MMU.

   Enables the MMU.

   @param l1Table Pointer to 16K aligned L1 translation table

   @see mmuNewL1Table
 */
extern void mmuEnable(void* l1Table);

/**
   @brief Enables MMU along with data and instruction caches.

   Enables MMU along with data and instruction caches. This uses a basic setup as per mmuNewL1Table() which 
   should be sufficient for most use cases.

   @see mmuNewL1Table
 */
extern void mmuCachesInitOn();

/**
   @brief Enable instruction cache.

   Enable instruction cache.
 */
extern void cacheEnableI();

/**
   @brief Disable instruction cache.

   Disable instruction cache.
 */
extern void cacheDisableI();

/**
   @brief Invalidate instruction cache.

   Invalidate instruction cache.
   @warning This does not write back any dirty data!
*/
extern void cacheInvalidateI();

/**
   @brief Enable data cache.

   Enable the data cache.
   @warning This will also enable the MMU or PU if it is not already enabled! Ensure you have set up the MMU or PU first.

   @see mmuEnable
 */
extern void cacheEnableD();

/**
   @brief Disable data cache.

   Disable data cache.
 */
extern void cacheDisableD();

/**
   @brief Invalidate data cache.

   Invalidate data cache.
   @warning This does not write back any dirty data!
*/
extern void cacheInvalidateD();

/**
   @brief Invalidate both data and instruction caches.

   Invalidate both data and instruction caches.
   @warning This does not write back any dirty data!
 */
extern void cacheInvalidateDI();


#endif
