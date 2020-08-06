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

/**
   @brief Access permissions for MMU domains.

   Access permissions for MMU domains.
 */
typedef enum {
	      /** Do not allow any access */ NO_ACCESS = 0x0,
	      /** Respect the permission bits in the translation table */ CLIENT = 0x1,
	      /** Ignore the permission bits in the translation table */ MANAGER = 0x3
} DomainAccess;

/**
   @brief Access permissions for MMU pages.

   Access permissions for MMU pages.
 */
typedef enum {
	      /** Read-write */ READ_WRITE = 0x0,
	      /** Read-only */ READ_ONLY = 0x2
} Access;

/**
   @brief Define access permissions.

   Define access permissions.

   @param access READ_ONLY or READ_WRITE
   @param el0Permitted Access is permitted when running in a non-privileged mode
   @see Access
*/
#define AP(access, el0Permitted) (access | (el0Permitted ? 1 : 0))

/**
   @brief Define a section descriptor.

   Define a section L1 descriptor (1MB).

   @param physicalAddress 1MB aligned physical address of section
   @param ap Access permissions
   @param domain Domain for this section (0 - 15)
   @param cacheable Section should be cacheable
   @param buffered Section should be buffered
   @see AP
 */
#define SECTION_DESCRIPTOR(physicalAddr, ap, domain, cacheable, buffered) (((physicalAddr) & 0xFFF00000) | (ap << 10) | (domain << 5) | BIT(4) | ((cacheable ? 1 : 0) << 3) | ((buffered ? 1 : 0) << 2) | 0x2)

// 256 entries for large (64k) or small (4k) pages
/**
   @brief Define a coarse descriptor.

   Define a coarse (256 entries) L1 descriptor for large (64K) or small (4K) pages.

   @param tablePhysicalAddr Physical address of L2 table
   @param domain Domain for this coarse descriptor (0 - 15)
 */
#define COARSE_DESCRIPTOR(tablePhysicalAddr, domain) ((tablePhysicalAddr & 0xFFFFFC00) | (domain << 5) | BIT(4) | 0x1)

/**
   @brief Define a fine descriptor.

   Define a fine (1024 entries) L1 descriptor for large (64K), small (4K) or tiny (1K) pages.

   @param tablePhysicalAddr Physical address of L2 table
   @param domain Domain for this fine descriptor (0 - 15)
 */
#define FINE_DESCRIPTOR(tablePhysicalAddr, domain) ((tablePhysicalAddr & 0xFFFFF000) | (domain << 5) | BIT(4) | 0x3)

/**
   @brief Define a large (64K) descriptor.

   Define a large (64K page) descriptor to go into an L2 table.

   @param physicalAddr 64K aligned physical address of page
   @param ap3 Access permissions for fourth 16K subpage
   @param ap2 Access permissions for third 16K subpage
   @param ap1 Access permissions for second 16K subpage
   @param ap0 Access permissions for first 16K subpage
   @param cacheable Section should be cacheable
   @param buffered Section should be buffered
   @see AP
 */
#define LARGE_DESCRIPTOR(physicalAddr, ap3, ap2, ap1, ap0, cacheable, buffered) ((physicalAddr & 0xFFFF0000) | (ap3 << 10) | (ap2 << 8) | (ap1 << 6) | (ap0 << 4) | ((cacheable ? 1 : 0) << 3) | ((buffered ? 1 : 0) << 2) | 0x1)

/**
   @brief Define a small (4K) descriptor.

   Define a small (4K page) descriptor to go into an L2 table.

   @param physicalAddr 4K aligned physical address of page
   @param ap3 Access permissions for fourth 4K subpage
   @param ap2 Access permissions for third 4K subpage
   @param ap1 Access permissions for second 4K subpage
   @param ap0 Access permissions for first 4K subpage
   @param cacheable Section should be cacheable
   @param buffered Section should be buffered
   @see AP
 */
#define SMALL_DESCRIPTOR(physicalAddr, ap3, ap2, ap1, ap0, cacheable, buffered) ((physicalAddr & 0xFFFFF000) | (ap3 << 10) | (ap2 << 8) | (ap1 << 6) | (ap0 << 4) | ((cacheable ? 1 : 0) << 3) | ((buffered ? 1 : 0) << 2) | 0x2)


/**
   @brief Define a tiny (1K) descriptor.

   Define a small (1K page) descriptor to go into an L2 table.

   @param physicalAddr 4K aligned physical address of page
   @param ap Access permissions
   @param cacheable Section should be cacheable
   @param buffered Section should be buffered
   @see AP
 */
#define TINY_DESCRIPTOR(physicalAddr, ap3, ap2, ap1, ap0, cacheable, buffered) ((physicalAddr & 0xFFFFFC00) | (ap << 4) | ((cacheable ? 1 : 0) << 3) | ((buffered ? 1 : 0) << 2) | 0x3)

/**
   @brief Protection Unit memory region sizes.

   Protection Unit memory region sizes.
 */
typedef enum {
	      PU_4K = 0b01011,
	      PU_8K = 0b01100,
	      PU_16K = 0b01101,
	      PU_32K = 0b01110,
	      PU_64K = 0b01111,
	      PU_128K = 0b10000,
	      PU_256K = 0b10001,
	      PU_512K = 0b10010,
	      PU_1M = 0b10011,
	      PU_2M = 0b10100,
	      PU_4M = 0b10101,
	      PU_8M = 0b10110,
	      PU_16M = 0b10111,
	      PU_32M = 0b11000,
	      PU_64M = 0b11001,
	      PU_128M = 0b11010,
	      PU_256M = 0b11011,
	      PU_512M = 0b11100,
	      PU_1G = 0b11101,
	      PU_2G = 0b11110,
	      PU_4G = 0b11111
} PUAreaSize;

/**
   @brief Define a PU memory region for the ARM940T.

   Define a PU memory region for the ARM940T.

   @note This does not apply to the ARM920T, where there is a full MMU.

   @see PUAreaSize
 */
#define PU_REGION(baseAddress, areaSize, enabled) ((baseAddress & 0xFFFFF000) | (areaSize << 1) | (enabled ? 1 : 0))

/**
   @brief Access permissions for PU memory regions.

   Access permissions for PU memory regions.
 */
typedef enum {
	      /** No access */ PU_NO_ACCESS = 0x0,
	      /** Only privileged mode access */ PU_PRIVILEGED_ONLY = 0x1,
	      /** Full access from privileged mode, read-only access from user mode */ PU_PRIVILEGED_RW_USER_RO = 0x2,
	      /** Full access from both privileged and user modes */ PU_FULL_ACCESS = 0x3
} PUAccess;

/**
   @brief Configures MMU domain access.

   Configures MMU domain access.

   @note ARM920T-only

   @param domain Domain to configure (0 - 15)
   @param access Access permissions to assign to domain
   @return State of Domain Access Control Register after the change
 */
extern uint32_t mmuSetDomainAccess(unsigned int domain, DomainAccess access);

/**
   @brief Allocates and populates a new L1 MMU table.

   Allocates and populates a new L1 MMU table. This will by default enable caching and buffering for the first 64M 
   of the address space (i.e. RAM) using 1M section descriptors set to domain 0.

   @note ARM920T-only

   @return A pointer to a correctly aligned L1 MMU table
 */
extern uint32_t* mmuNewL1Table();

/**
   @brief Enable the MMU.

   Enable the MMU.

   @note ARM920T-only

   @param l1Table Pointer to 16K aligned L1 translation table

   @see mmuNewL1Table
 */
extern void mmuEnable(void* l1Table);

/**
   @brief Disable the MMU.

   Disable the MMU.

   @note This will also disable the data cache.
   @note ARM920T-only
 */
extern void mmuDisable();

/**
   @brief Enables MMU along with data and instruction caches.

   Enables MMU along with data and instruction caches. This uses a basic setup as per mmuNewL1Table() which 
   should be sufficient for most use cases.

   @note ARM920T-only

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

/**
   @brief Enable the PU.

   Enable the PU.

   @note You must have configured a region before calling this.

   @see puSetDRegion
   @see puSetIRegion
 */
extern void puEnable();

/**
   @brief Disable the PU.

   Disable the PU.

   @note This will also disable the data cache.
 */
extern void puDisable();

/**
   @brief Configure a PU data memory region.

   Configure a PU data memory region.

   @param region Region to figure (0 - 7)
   @param params Basic region params (defined using PU_REGION)
   @param access Access permissions for region
   @param cacheable Whether region is cacheable
   @param buffered Whether region is buffered
   @see PU_REGION
 */
extern void puSetDRegion(unsigned int region, uint32_t params, PUAccess access, bool cacheable, bool buffered);

/**
   @brief Configure a PU instruction memory region.

   Configure a PU instruction memory region.

   @note The instruction cache is not bufferable.

   @param region Region to figure (0 - 7)
   @param params Basic region params (defined using PU_REGION)
   @param access Access permissions for region
   @param cacheable Whether region is cacheable
   @see PU_REGION
 */
extern void puSetIRegion(unsigned int region, uint32_t params, PUAccess access, bool cacheable);

#endif
