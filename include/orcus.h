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

extern int sdInit();
extern int sdReadBlocks(int startBlock, int numberOfBlocks, uint8_t* dest);
extern bool sdIsInserted();
extern int sdSizeKb();

/**
   @brief Check if running on an F200.

   Check if running on an F200. This tests to see if the NET2272 chip (only fitted to F100s) is present.

   @return true if running on an F200, false if running on an F100
 */
extern bool gp2xIsF200();

// timer increments once every 0.135uS and overflows at 0xFFFFFFFF then carries on counting
#define TIMER_NS_PER_TICK 135
extern uint32_t timerGet();
extern uint32_t timerSet(uint32_t count);
extern void timerSleepNs(unsigned int ns);
extern unsigned int timerNsSince(uint32_t lastTick, uint32_t* storeCurrent);

extern void nandRead(uint32_t startAdddr, int numberOfBlocks, void* dest);
extern void nandErase(uint32_t startAdddr, int numberOfBlocks);
extern void nandWrite(uint32_t startAddr, int numberOfBlocks, void* src);

extern void enableICache();

extern int arm940Init(int memoryBank);
extern bool arm940Running();
extern void arm940Halt();
extern void arm940Run();
extern void arm940ClockOn();
extern void arm940ClockOff();
extern bool arm940HasClock();
extern bool arm940IsThis();
extern void* importPointer(uint32_t addr);
extern uint32_t exportPointer(void* ptr);
extern volatile uint16_t* arm940Data;
extern volatile uint16_t* arm920Data;

#endif
