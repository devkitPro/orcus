/*! \file rgb.h
    \brief RGB
 */

#ifndef __ORCUS_RGB_H__
#define __ORCUS_RGB_H__

#include <stdint.h>

typedef uint8_t uint4_t;

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
	      /** 4-bit palette mode, each 4 bit value (X, Y) is an index into the palette 0xXY */ P4BPP = 0,
	      /** 8-bit palette mode, each byte is an index into the palette */ P8BPP = 1,
	      /** 16-bit RGB565 mode */ RGB565 = 2,
	      /** 24-bit RGB888 mode */ RGB888 = 3,
	      /** 1-bit B/W mode */ B1BPP = 4
} RgbFormat;

/**
   Blending modes for combining regions on-screen.
 */
typedef enum {
	      /** No blending between regions */ NO_BLENDING = 0,
	      /** Exclude pixels of a particular colour */ COLOUR_KEY = 1,
	      /** Alpha blending */ ALPHA = 2
} BlendingMode;

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

   @note REGION5 cannot be moved or resized, it is always 320x240 starting at (0, 0).

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
   @brief Set colour palette for 4 or 8 bit modes.

   Set colour palette for 4 or 8 bit modes. A palette can be up to 256 entries long (only the first 16 are used in 4bpp mode).

   @param colours Pointer to array of colour data in RGB888x format
   @param count Number of palette entries to set
   @param startIdx First palette index to set
 */
extern void rgbSetPalette(uint32_t* colours, int count, uint8_t startIdx);

#endif
