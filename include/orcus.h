/*! \file orcus.h
    \brief Main header file for Orcus functions
 */

#ifndef __ORCUS_H__
#define __ORCUS_H__

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t uint4_t;

/**
   @brief Initialise GP2X

   Initialise the GP2X hardware and set up system calls. This must be called before using any other functions from Orcus.
 */
extern void gp2xInit();

/**
   @brief Set custom RAM timings.

   Sets RAM timing registers in MMSP2.
 */
extern void gp2xSetRamTimings(int tRC, int tRAS, int tWR, int tMRD, int tRFC, int tRP, int tRCD);

/**
   @brief Set default recommended RAM timings.

   Equivalent to gp2xSetRamTimings(7, 15, 2, 7, 7, 7, 7)
   @see gp2xSetRamTimings
 */
extern void gp2xSetDefaultRamTimings();

/**
   @brief Set faster RAM timings.

   Equivalent to gp2xSetRamTimings(5, 3, 0, 0, 0, 1, 1)
   @see gp2xSetRamTimings
 */
extern void gp2xSetFastRamTimings();

/**
   UART parity settings.
 */
typedef enum {
	      NONE /** No parity bit */,
	      ODD /** Odd parity bit */,
	      EVEN /** Even parity bit */
} Parity;

/**
   @brief Configure UART parameters.

   Configures UART parameters.

   Example (115200 8N1):
   @code
   uartConfigure(115200, 8, NONE, 1)
   @endcode
 */
extern void uartConfigure(int baudRate, int bitsPerFrame, Parity parity, int stopBits);

/**
   @brief Output character over UART.

   Outputs a single character over UART.

   @param c Character to output
   @param isBlocking If true will wait until the output buffer has space for this character, will fail fast if false
   @returns The character sent if successful. If non-blocking operation is used and the buffer is full, -1.
 */
extern char uartPutc(char c, bool isBlocking);

/**
   @brief Get a character from UART.

   Gets a single character from UART.

   @param isBlocking If true, will wait for a character to become available, otherwise will return immediately if no character is in the input buffer.
   @return The next character from the input buffer. If non-blocking operation is used and the buffer is empty, EOF.
 */
extern int uartGetc(bool isBlocking);

/**
   @brief printf over UART.

   As per the standard printf, with output directly to UART.

   @note This has a maximum output buffer of 256 bytes so you may be better off using the newlib printf.
 */
extern void uartPrintf(const char* format, ...);

/**
   @brief Set UART echoing.

   If enabled, uartGetc will echo any character received back out to the UART. This is handy when implementing a command line interface over UART.

   @param isEnabled If true, UART echoing will be enabled.
 */
extern void uartSetEcho(bool isEnabled);

/**
   Regions available in RGB hardware.
 */
typedef enum {
	      /** First RGB region */ REGION1 = 1,
	      /** Second RGB region */ REGION2 = 2,
	      /** Third RGB region */ REGION3 = 3,
	      /** Fourth RGB region */ REGION4 = 4,
	      /** Fifth RGB region */ REGION5 = 5
} RgbRegion;

/**
   Bit formats in RGB region.
 */
typedef enum {
	      /** 4-bit palette mode, each 4 bit value (X, Y) is an index into the palette 0xXY */P4BPP = 0,
	      /** 8-bit palette mode, each byte is an index into the palette */ P8BPP = 1,
	      /** 16-bit RGB565 mode */ RGB565 = 2,
	      /** 24-bit RGB888 mode */ RGB888 = 3
} RgbFormat;

/**
   Blending modes for combining regions on-screen.
 */
typedef enum {
	      /** No blending between regions */ NO_BLENDING = 0,
	      /** Exclude pixels of a particular colour */ COLOUR_KEY = 1,
	      /** Alpha blending */ ALPHA = 2
} BlendingMode;

///@{
/** Button */
#define A (1<<0)
#define B (1<<1)
#define X (1<<2)
#define Y (1<<3)
#define START (1<<4)
#define SELECT (1<<5)
#define UP (1<<6)
#define DOWN (1<<7)
#define LEFT (1<<8)
#define RIGHT (1<<9)
#define UP_LEFT (1<<10)
#define UP_RIGHT (1<<11)
#define DOWN_LEFT (1<<12)
#define DOWN_RIGHT (1<<13)
#define VOL_UP (1<<14)
#define VOL_DOWN (1<<15)
#define STICK (1<<16)
#define L (1<<17)
#define R (1<<18)
///@}

/**
   @brief Configure RGB pixel format for all regions.

   Configure RGB pixel format for all regions.
   
   @param format Format to use when interpreting RGB data
 */
extern void rgbSetPixelFormat(RgbFormat format);

/**
   @brief Enable or disable an RGB region.

   Enable or disable an RGB region.

   @param region Region to alter
   @param onOff true to enable a region, false to disable it
 */
extern void rgbToggleRegion(RgbRegion region, bool onOff);

/**
   @brief Enable alpha blending for this region.

   Enable alpha blending for this region, and set alpha value.

   @param region Region to alter
   @param alpha Alpha value (0 - 16)
 */
extern void rgbRegionBlendAlpha(RgbRegion region, uint4_t alpha);

/**
   @brief Enable colour key blending for region.

   Enable colour key blending for region.

   @param region Region to alter
 */
extern void rgbRegionBlendColourKey(RgbRegion region);

/**
   @brief Set global colour key for blending.

   Set global colour key for blending RGB regions (all regions share the same key).

   @param r Red value to match
   @param g Green value to match
   @param b Blue value to match
 */
extern void rgbColourKey(uint8_t r, uint8_t g, uint8_t b);

/**
   @brief Disable blending for a region.

   Disable blending for a region.

   @param region Region to alter
 */
extern void rgbRegionNoBlend(RgbRegion region);

/**
   @brief Set source with for scaling region.

   Sets source width for scaling region.

   @param srcW Source width in pixels, scaling in X dimension disabled if set to 0
   @param srcH Source height in pixels, scaling in Y dimension disabled if set to 0
 */
extern void rgbSetScale(int srcW, int srcH);

/**
   @brief Set RGB framebuffer address.

   Set RGB framebuffer address. The regions all share the same base pointer and are accessed sequentially from that location.

   @param fb Pointer to memory region which should be used as source of RGB data
*/
extern void rgbSetFbAddress(void* fb);

/**
   @brief Set region coordinates and size.

   Set region coordinates and size.

   @param region Region to alter
   @param x X-coordinate of start of region
   @param y Y-coordinate of start of region
   @param width Width in pixels of region
   @param height Height in pixels of region
 */
extern void rgbSetRegionPosition(RgbRegion region, int x, int y, int width, int height);

/**
   @brief Set font data to use.

   Set font data to use for text drawing utility functions.

   @note This only works in RGB565 mode.

   @param font Pointer to font graphic data
   @param charWidth Width of font character in pixels
   @param charHeight Height of font character in pixels
 */
extern void rgbSetFont(uint16_t* font, int charWidth, int charHeight);

/**
   @brief Render a font character on the screen.

   Render a font character on the screen. Must have configured font first using rgbSetFont.

   @note This only works in RGB565 mode.

   @param fb Pointer to RGB565 framebuffer in which to render character
   @param x X-coordinate at which to draw font
   @param y Y-coordinate at which to draw font
   @param colour RGB565 colour to draw character
   @param c Character to draw
   @see rgbSetFont
   @see rgbPutcBg
 */
extern void rgbPutc(uint16_t* fb, int x, int y, uint16_t colour, char c);

/**
   @brief Render a font character on the screen with a background colour.

   Render a font character on the screen with a background colour. Must have configured font first using rgbSetFont.

   @note This only works in RGB565 mode.

   @param fb Pointer to RGB565 framebuffer in which to render character
   @param x X-coordinate at which to draw font
   @param y Y-coordinate at which to draw font
   @param colour RGB565 colour to draw character
   @param bgColour RGB565 colour to use for background
   @param c Character to draw
   @see rgbSetFont
   @see rgbPutc
 */
extern void rgbPutcBg(uint16_t* fb, int x, int y, uint16_t colour, uint16_t bgColour, char c);

/**
   @brief printf directly to RGB565 framebuffer.

   printf directly to RGB565 framebuffer. Must have configured font first using rgbSetFont.

   @note This only works in RGB565 mode.
   @note This has a maximum output buffer of 256 bytes.

   @param fb Pointer to RGB565 framebuffer in which to render character
   @param x X-coordinate at which to draw font
   @param y Y-coordinate at which to draw font
   @param colour RGB565 colour to draw character

   @see rgbSetFont
   @see rgbPrintfBg
 */
extern void rgbPrintf(uint16_t* fb, int x, int y, uint16_t colour, const char* format, ...);

/**
   @brief printf directly to RGB565 framebuffer with a background colour.

   printf directly to RGB565 framebuffer with a background colour. Must have configured font first using rgbSetFont.

   @note This only works in RGB565 mode.
   @note This has a maximum output buffer of 256 bytes.

   @param fb Pointer to RGB565 framebuffer in which to render character
   @param x X-coordinate at which to draw font
   @param y Y-coordinate at which to draw font
   @param colour RGB565 colour to draw character
   @param bgColour RGB565 colour to use for background

   @see rgbSetFont
   @see rgbPrintf
 */
extern void rgbPrintfBg(uint16_t* fb, int x, int y, uint16_t colour, uint16_t bgColour, const char* format, ...);

/**
   @brief Check if LCD is currently in vsync period.

   Check if LCD is currently in vsync period.

   @return true if LCD is currently in vsync period, otherwise false
 */
extern bool lcdVSync();

/**
   @brief Wait until LCD is in next vsync period.

   Wait until LCD is in next vsync period. If LCD is currently in vsync when this is called, it will wait until it has ended, and then again until the next one begins.
 */
extern void lcdWaitNextVSync();

/**
   @brief Check if LCD is currently in hsync period.

   Check if LCD is currently in hsync period.

   @return true if LCD is currently in hsync period, otherwise false
 */
extern bool lcdHSync();

/**
   @brief Wait until LCD is in next hsync period.

   Wait until LCD is in next hsync period. If LCD is currently in hsync when this is called, it will wait until it has ended, and then again until the next one begins.
 */
extern void lcdWaitNextHSync();

/**
   @brief Get current button state.

   Get current button state (raw).

   One can check for a particular button state using &.

   For example:

   @code
   (btnState() & START)
   @endcode

   @return Currently pressed buttons OR-ed together.
   @see A
 */
extern uint32_t btnState();

/**
   @brief Get current button state (debounced).

   Get current button state. This function debounces presses to give a reliable indicator.

   One can check for a particular button state using &.

   For example:

   @code
   (btnStateDebounced() & START)
   @endcode

   @return Currently pressed buttons OR-ed together.
   @see A
 */
extern uint32_t btnStateDebounced();

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
   @brief Check if SD card is inserted.

   Check if an SD card is physically inserted into the slot (reads the state of the mechanical switch).

   @return true if an SD card is inserted, false otherwise
 */
extern bool sdIsInserted();

/**
   @brief Check if running on an F200.

   Check if running on an F200. This tests to see if the NET2272 chip (only fitted to F100s) is present.

   @return true if running on an F200, false if running on an F100
 */
extern bool gp2xIsF200();

// timer increments once every 0.135uS and overflows at 0xFFFFFFFF then carries on counting
/**
   @def TIMER_NS_PER_TICK
   @brief Nanoseconds per timer tick.

   Nanoseconds per timer tick. Timer ticks once every 0.135uS.
 */
#define TIMER_NS_PER_TICK 135

/**
   @brief Get current timer tick.

   Get current timer tick.

   Timer ticks every 0.135uS and overflows at 0xFFFFFFFF then carries on counting.

   @return Current timer tick
 */
extern uint32_t timerGet();

/**
   @brief Reset timer to count from specific tick.

   Reset timer to count from specific tick.

   @note There is a bug in the MMSP2 silicon which prevents starting the timer from 0, so this function will start it from 1 if you try to pass 0.

   @param count Tick to start counting from
   @return Tick the timer was at before being reset
 */
extern uint32_t timerSet(uint32_t count);

/**
   @brief Wait for a given number of nanoseconds.

   Wait for a given number of nanoseconds.

   @note This function won't work correctly if you try to wait for longer than 579.82s due to timer overflow.

   @param ns Nanoseconds to wait
 */
extern void timerSleepNs(unsigned int ns);

/**
   @brief Calculate nanoseconds elapsed since a given tick.

   Calculate nanoseconds elapsed since a given tick. Optionally store the current tick.

   Useful for timeouts, or calculating time between frames.

   @note This function won't work correctly for periods longer than 579.82s due to timer overflow.
 */
extern unsigned int timerNsSince(uint32_t lastTick, uint32_t* storeCurrent);

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

/**
   @brief Enable icache on ARM920T

   Enable icache on ARM920T. This is enabled by default in gp2xInit.
   @warning This may have unexpected effects if called from the ARM940T
 */
extern void enableICache();

/**
   @brief Initialise ARM940T.

   Initialise the ARM940T second CPU and configure the base address (in 16M increments) in physical memory which will appear as 0x0 from the perspective of the ARM940T.

   This leaves the ARM940T in RESET state, and enables the clock.

   Valid base addresses are 16M, 32M and 48M (banks 1, 2 and 3 respectively) to prevent overlapping with the ARM920T vector table (always stored at 0x0 in physical memory). It is possible to set both CPUs to start at 0x0 physical but in this case they will share a RESET vector and will execute the same code so it is highly discouraged unless you have an unusual use case in mind, and is not supported with this function.

   Further documentation can be found on the Orcus wiki regarding using the ARM940T.

   @note This should only be called from the ARM920T.

   @param memoryBank Memory bank to use as start of ARM940T address range (1 - 3)
   @return 0 if successful, non-zero otherwise
 */
extern int arm940Init(int memoryBank);

/**
   @brief Check if the ARM940T is running.

   Check if the ARM940T is running.

   @note It makes no sense to call this from the ARM940T.

   @return true if the ARM940T is running, false if it is being held in RESET state
 */
extern bool arm940Running();

/**
   @brief Halt the ARM940T.

   Halt the ARM940T and hold it in RESET state.

   @note You can call this from the ARM940T but you won't get control back afterwards.
 */
extern void arm940Halt();

/**
   @brief Start execution on the ARM940T.

   Start execution on the ARM940T from its RESET vector entry (0x0) by releasing it from RESET state.

   @note It makes no sense to call this from the ARM940T.
 */
extern void arm940Run();

/**
  @brief Resume execution on the ARM940T.

  Enable the clock on the ARM940T to resume execution at the same point it was at when the clock was disabled.

  @note It makes no sense to call this from the ARM940T.

  @see arm940ClockOff
 */
extern void arm940ClockOn();

/**
   @brief Pause execution on the ARM940T.

   Pause execution on the ARM940T by disabling its clock.

   @note You can call this from the ARM940T but you won't get control back until the ARM920 calls arm940ClockOn()

   @see arm940ClockOn
 */
extern void arm940ClockOff();

/**
   @brief Check if the ARM940T has a clock signal.

   Check if the ARM940T has a clock signal, to see if execution is paused.

   @return true if the ARM940T has a clock signal, false otherwise (i.e. it is paused)
 */
extern bool arm940HasClock();

/**
   @brief Check if executing on the ARM940T.

   Check if this call is being executed on the ARM940T.

   @return true if this is called from the ARM940T, false if called from the ARM920T
 */
extern bool arm940IsThis();

/**
   @brief Import a pointer from the other CPU.

   Import a pointer from the other CPU into the address space of the CPU we are running on.

   @warning It may not be possible to import a pointer from memory which is only viewable from the ARM920T when on the ARM940T. It is always possible to import from the ARM940T to the ARM920T however.

   @param addr Address to import from other CPU
   @return Pointer to the given address within the address space of the current CPU
 */
extern void* importPointer(uint32_t addr);

/**
   @brief Export a pointer to the other CPU.

   Export a pointer to the address space of the other CPU.

   @warning It may not be possible to export a pointer from memory which is only viewable from the ARM920T to the ARM940T. It is always possible to export from the ARM940T to the ARM920T however.

   @param ptr Pointer to export
   @return Address of ptr within the address space of the other CPU
 */
extern uint32_t exportPointer(void* ptr);

/**
   @var arm940Data
   @brief Pointer to start of 16x 16-bit shared data registers.

   Pointer to start of 16x 16-bit shared data registers. These can be written to from either CPU, but can only trigger an interrupt on the ARM940T.
 */
extern volatile uint16_t* arm940Data;
/**
   @var arm920Data
   @brief Pointer to start of 16x 16-bit shared data registers.

   Pointer to start of 16x 16-bit shared data registers. These can be written to from either CPU, but can only trigger an interrupt on the ARM920T.
 */
extern volatile uint16_t* arm920Data;

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

/**
   @brief Start AC97 codec.

   Start AC97 codec, will restart codec if it was previously started.

   @note DMA channels must have been set up before calling this.
 */
extern void ac97Start();

/**
   @brief Stop AC97 codec.

   Stop AC97 codec.
 */
extern void ac97Stop();

/**
   @brief Read register value from AC97 codec.

   Read register value from AC97 codec

   @note Must have called ac97Start() or audioInit() before using this.

   @param reg Register to read
   @return Register value
   @see ac97Start
   @see audioInit
 */
extern uint16_t ac97GetReg(uint8_t reg);

/**
   @brief Set register value in AC97 codec.

   Set register value in AC97 codec.

   @note Must have called ac97Start()  or audioInit() before using this.

   @param reg Register to set
   @param value Value to set
   @see ac97Start
   @see audioInit
*/
extern void ac97SetReg(uint8_t reg, uint16_t value);

/**
   @brief Initialise audio subsystem.

   Initialise auto subsystem.

   @param dmaChannel DMA channel to use for audio (0 - 15, recommend 0 - 3)
 */
extern void audioInit(int dmaChannel);

/**
   @brief Set sample rate for AC97 codec.
   
   Set sample rate for AC97 codec.

   @note Must have called audioInit() before using this.

   @param khz Sample rate to set
   @see ac97Start
   @see audioInit
 */
extern void audioSetSampleRate(uint16_t khz);

/**
   @brief Mute speakers.

   Mute speakers.
 */
extern void audioMuteSpeakers();

/**
   @brief Unmute speakers.

   Unmute speakers.
 */
extern void audioUnmuteSpeakers();

/**
   @brief Mute headphones.

   Mute headphones.
 */
extern void audioMuteHeadphones();

/**
   @brief Unmute headphones.

   Unmute headphones.
 */
extern void audioUnmuteHeadphones();

/**
   @brief Set volume.

   Set volume.

   @warn You may not wish to allow the volume to reach 31, with headphones connected this could do damage to somebody's hearing!

   @param left Volume level for left channel (0 [lowest] - 31 [highest])
   @param right Volume level for right channel (0 [lowest] - 31 [highest])
 */
extern void audioSetVolume(uint8_t left, uint8_t right);

/**
   @brief Play a PCM sample.

   Play a PCM sample. Non-blocking, see if the sample has finished playing with audioSamplePlaying().

   @param bytes Number of bytes in sample (max 0xFFFF)
   @param data Pointer to sample data
   @see audioSamplePlaying
 */
extern void audioPlaySample(uint16_t bytes, void* data);

/**
   @brief Check if a sample is currently playing.

   Check if a sample is currently playing.

   @return true if a sample is currently playing, false otherwise
 */
extern bool audioSamplePlaying();

/**
   @brief Check if headphones are connected (F100 only).

   Check if headphones are connected. This works on the F100 only, the F200 physically switches the DAC output between the speakers and headphones when you insert the jack.

   @note F100 only
   @return true if headphones are connected, false otherwise
 */
extern bool audioHeadphonesConnected();

#endif
