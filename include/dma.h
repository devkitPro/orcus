/*! \file dma.h
    \brief DMA
 */

#ifndef __ORCUS_DMA_H__
#define __ORCUS_DMA_H__

#include <stdint.h>

/**
   DMA burst mode (RAM->RAM only).
 */
typedef enum {
	      NO_BURST = 0x0, /** Copy one word at a time */
	      WORDS_4 = 0x1 /** Copy 4 words at a time */,
	      WORDS_8 = 0x2 /** Copy 8 words at a time */
} BurstMode;

/**
   MMSP2 peripherals available for DMA transfers.
*/
typedef enum {
	      AC97_LRPCM = 24
} Peripheral;

/**
   @brief Configure a DMA channel for memory-to-memory transfer.

   Configure a DMA channel for memory-to-memory transfer.

   @param channel DMA channel to configure (0 - 15)
   @param burstMode Number of words to copy at a time
   @param srcIncrement Number of words to increment source address after each transfer (negative values and zero are permitted)
   @param destIncrement Number of words to increment destination address after each transfer (negative values and zero are permitted)
 */
extern void dmaConfigureChannelMem(int channel, BurstMode burstMode, int8_t srcIncrement, int8_t destIncrement);

/**
   @brief Configure a DMA channel for memory-to-peripheral transfer.

   Configure a DMA channel for memory-to-peripheral transfer.

   @param channel DMA channel to configure (0 - 15)
   @param burstMode Number of words to copy at a time
   @param srcIncrement Number of words to increment source address after each transfer (negative values and zero are permitted)
   @param destIncrement Number of words to increment destination address after each transfer (negative values and zero are permitted)
   @param peripheral MMSP2 peripheral to write to
 */
extern void dmaConfigureChannelIO(int channel, BurstMode burstMode, int8_t srcIncrement, int8_t destIncrement, Peripheral peripheral);


/**
   @brief Initiate a DMA transfer.

   Initiate a DMA transfer on a previously configured channel.

   @param channel DMA channel to initiate transfer on (0 - 15)
   @param length Number of bytes to transfer (maximum 64K)
   @param src Source address (aligned to 4 byte word boundary)
   @param dst Destination address (aligned to 4 byte word boundary)
   @see dmaConfigureChanelMem
 */
extern void dmaStart(int channel, uint16_t length, uint32_t src, uint32_t dest);

/**
   @brief Stop a DMA transfer.

   Stop a DMA transfer.

   @param channel DMA channel to stop (0 - 15)
 */
extern void dmaStop(int channel);

/**
   @brief Check if a DMA channel is transferring data.

   Check if a DMA channel is transferring data.

   @param channel DMA channel to check (0 - 15)
 */
extern bool dmaIsTransferring(int channel);

/**
   @brief Check if a DMA channel has finished transferring data.

   Check if a DMA channel has finished transferring data.

   @param channel DMA channel to check (0 - 15)
 */
extern bool dmaHasFinished(int channel);

#endif
