/*! \file sd.h
    \brief SD card
 */

#ifndef __ORCUS_SD_H__
#define __ORCUS_SD_H__

#include <stdint.h>

/**
   @brief Initialise SD card.

   Initialise SD card.

   @return 0 if successful, non-zero otherwise
 */
extern int sdInit();

/**
   @brief Read blocks from SD card.

   Read blocks (512B) from SD card.

   @note Must have called sdInit first

   @param startBlock Block to start reading from (i.e. address / 512)
   @param numberOfBlocks Number of 512B blocks to read
   @param dest Pointer to memory location to store data
   @return 0 if successful, non-zero otherwise

   @see sdInit
 */
extern int sdReadBlocks(int startBlock, int numberOfBlocks, uint8_t* dest);

/**
   @brief Write blocks to SD card.

   Write blocks (512B) to SD card.

   @note Must have called sdInit first

   @param startBlock Block to start reading writing at (i.e. address / 512)
   @param numberOfBlocks Number of 512B blocks to write
   @param dest Pointer to memory location of data to write
   @return 0 if successful, non-zero otherwise

   @see sdInit
 */
extern int sdWriteBlocks(int startBlock, int numberOfBlocks, uint8_t* src);

/**
   @brief Check if SD card is inserted.

   Check if an SD card is physically inserted into the slot (reads the state of the mechanical switch).

   @return true if an SD card is inserted, false otherwise
 */
extern bool sdIsInserted();

#endif
