/*! \file nand.h
    \brief NAND
 */

#ifndef __NAND_H__
#define __NAND_H__

#include <stdint.h>

/**
   @def NAND_BLOCK_SIZE

   @brief Number of bytes in a NAND block.
 */
#define NAND_BLOCK_SIZE 512

/**
   @brief Read blocks from NAND.

   Read 512B blocks from NAND.

   @param startAddr Address to start reading from (absolute, not in terms of blocks)
   @param numberOfBlocks Number of 512B blocks to read
   @param dest Pointer to memory location to store data
 */
extern void nandRead(uint32_t startAddr, int numberOfBlocks, void* dest);

/**
   @brief Erase blocks on NAND.

   Erase (set to 0xFF) 512B blocks on NAND. You must do this before writing due to the nature of NAND storage where a write operating can only unset bits.

   @warning You can brick a GP2X if you erase the first 512K of NAND, where the bootloader is stored.

   @param startAddr Address to start erasing from (absolute, not in terms of blocks)
   @param numberOfBlocks Number of 512B blocks to erase

   @see nandWrite
 */
extern void nandErase(uint32_t startAdddr, int numberOfBlocks);

/**
   @brief Write blocks to NAND.

   Write 512B blocks to NAND. You must first erase the blocks.

   @warning You can brick a GP2X if you write to the first 512K of NAND, where the bootloader is stored.

   @param startAddr Address to start start writing to (absolute, not in terms of blocks)
   @param numberOfBlocks Number of 512B blocks to write
   @param src Pointer to memory location holding data to write

   @see nandWrite
 */
extern void nandWrite(uint32_t startAddr, int numberOfBlocks, void* src);

#endif
