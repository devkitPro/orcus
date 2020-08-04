/*! \file 2d.h
    \brief 2D accelerator support
 */

#ifndef __2D_H__
#define __2D_H__

#include <orcus.h>

/**
   Rotation angle.
 */
typedef enum {
	      /** 0 degrees of rotation */ DEG0 = 0,
	      /** 90 degrees of rotation */ DEG90 = 1,
	      /** 180 degrees of rotation */ DEG180 = 2,
	      /** 270 degrees of rotation */ DEG270 = 3
} Angle;

/**
   Defines a graphic, be it on or off-screen.
 */
typedef struct {
  /** Pointer to graphic data */ const void* data;
  /** Width in pixels of graphic */ int w;
  /** Height in pixels of graphic  */int h;
  /** RGB data format of graphic data */ RgbFormat format;
} Graphic;

/**
   Defines a rectangular area.
 */
typedef struct {
  /** X-coordinate of upper left corner of area */ int x;
  /** Y-coordinate of upper left corner of area */ int y;
  /** Width in pixels of area */ int w;
  /** Height in pixels of area */int h;
} Rect;

/**
   Defines the pattern setup for raster operations.
 */
typedef struct {
  /** 16-bit RGB565 or 8 pit palette foreground colour where pattern is 1bpp */ uint16_t fgCol;
  /** 16-bit RGB565 or 8 pit palette background colour where pattern is 1bpp */ uint16_t bgCol;
  /** RGB data format of pattern data (only P8BPP, RGB565 and B1BPP are valid) */ RgbFormat format; // only 8bpp, 16bpp or 1bpp are valid
  /** Offset pattern by the given number of pixels in the Y direction */ int yOffset;
} RasterPattern;

///@{
/** Pre-defined GDI ternary raster operation, the full list can be found on the 
    <a href="https://docs.microsoft.com/en-us/windows/win32/gdi/ternary-raster-operations">Microsoft website</a>.
 */
#define ROP_SRCCOPY 0xCC // blit
#define ROP_PATCOPY 0xF0 // fill
#define ROP_DPSao 0xEA // exclude pixels defined in the pattern
#define ROP_SPno 0xCF
///@}

/**
   @var pattern
   @brief Pointer to start of pattern graphic data.

   Pointer to the start of the 8x8 graphic data. This is 128B long to accommodate an 8x8 RGB565 pattern.
 */
extern volatile uint32_t* pattern;

/**
   @brief Set the transparent colour.

   Sets the colour to be considered transparent in a source when performing a raster operation - this colour will be 
   omitted. By default this is set to magenta (0xF81F in RGB565).
 */
extern void rgbSetTransparencyColour(uint16_t colour);

/**
   @brief Configure a blit raster operation.

   Configure a blit raster operation.

   @param src Source graphic to copy
   @param srcRect Area of source graphic to copy
   @param dest Destination graphic to copy onto
   @param x X-coordinate of upper left hand corner of where the source graphic should be drawn on the destination
   @param y Y-coordinate of upper left hand corner of where the source graphic should be drawn on the destination
   @param enableTransparency The transparent colour will be omitted from the copy if true

   @see rgbSetTransparencyColour
 */
extern void rgbBlit(Graphic* src, Rect* srcRect, Graphic* dest, int x, int y, bool enableTransparency);

/**
   @brief Configure a 1bpp blit raster operation.

   Configure a 1bpp blit raster operation. Where the source image is 1bpp, the 2d 
   accelerator will convert it to the destination format by using the supplied foreground and background colours.

   @param src Source graphic to copy
   @param srcRect Area of source graphic to copy
   @param dest Destination graphic to copy onto
   @param x X-coordinate of upper left hand corner of where the source graphic should be drawn on the destination
   @param y Y-coordinate of upper left hand corner of where the source graphic should be drawn on the destination
   @param enableTransparency The transparent colour will be omitted from the copy if true
   @param fgCol Foreground colour in the destination format
   @param bgCol Background colour in the destination format

   @see rgbSetTransparencyColour
 */
extern void rgbBlit1bpp(Graphic* src, Rect* srcRect, Graphic* dest, int x, int y, bool enableTransparency, uint16_t fgCol, uint16_t bgCol);

/**
   @brief Configure a pattern fill raster operation.

   Configure a pattern fill raster operation, where a region on the destination will be filled with the repeated pattern.

   @param dest Destination graphic to fill
   @param region Region of destination to fill
   @param pattern Configuration of pattern to fill with
   @param enableTransparency The transparent colour will be omitted from the fill
 */
extern void rgbPatternFill(Graphic* dest, Rect* region, RasterPattern* pattern, bool enableTransparency);

/**
   @brief Configure a solid fill raster operation.

   Configure a solid fill raster operation.

   @param dest Destination graphic to fill
   @param region Region of destination to fill
   @param colour Colour to fill area with in destination format
 */
extern void rgbSolidFill(Graphic* dest, Rect* region, uint16_t colour);

/**
   @brief Configure a raster operation.

   Configure a raster operation. This is a lower level function which is used by all the other raster functions, one can use this to configure a custom ternary raster operation which can combine source, pattern and destination pixels in a variety of ways (see the <a href="https://docs.microsoft.com/en-us/windows/win32/gdi/ternary-raster-operations">Microsoft documentation</a> for full details on GDI ternary raster operations).

   @param src Source graphic to copy (NULL if not used)
   @param srcRect Area of source graphic to copy (NULL if not used)
   @param dest Destination graphic to render onto
   @param destRect Area of destination graphic to render onto
   @param pattern Configuration of pattern (NULL if not used)
   @param enableTransparency The transparent colour will be omitted from the copy if true
   @param srcFgCol Foreground colour to use when expanding 1bpp source, in the destination format
   @param srcBgCol Background colour to use when expanding 1bpp source, in the destination format

   @see rgbSetTransparencyColour
   @see ROP_PATCOPY
 */
extern void rgbRasterOp(Graphic* src, Rect* srcRect, Graphic* dest, Rect* destRect, uint8_t rasterOp, RasterPattern* pattern, bool enableTransparency, uint16_t srcFgCol, uint16_t srcBgCol);

/**
   @brief Start running pre-configured raster operation.

   Start running pre-configured raster operation.
 */
extern void rgbRasterRun();

/**
   @brief Check if a raster operation is currently being run.

   Check if a raster operation is currently being run.

   @return true if a raster operation is currently being run, false otherwise
 */
extern bool rgbRasterIsRunning();

/**
   @brief Wait until the current raster operation has completed.

   Wait until the current raster operation has completed.
 */
extern void rgbRasterWaitComplete();

/**
   @brief Configure a rotation blit operation.

   Configure a rotation blit operation.

   @param src Source graphic to copy
   @param srcRect Area of source graphic to copy
   @param dest Destination graphic to copy onto
   @param x X-coordinate of upper left hand corner of where the source graphic should be drawn on the destination
   @param y Y-coordinate of upper left hand corner of where the source graphic should be drawn on the destination
   @param angle The angle to rotate the source image by when drawing onto the destination
 */
extern void rgbRotBlit(Graphic* src, Rect* srcRect, Graphic* dest, int x, int y, Angle angle);

/**
   @brief Start running pre-configured rotation operation.

   Start running pre-configured rotation operation.
 */
extern void rgbRotRun();

/**
   @brief Check if a rotation operation is currently being run.

   Check if a rotation operation is currently being run.

   @return true if a rotation operation is currently being run, false otherwise
 */
extern bool rgbRotIsRunning();

/**
   @brief Wait until the current rotation operation has completed.

   Wait until the current rotation operation has completed.
 */
extern void rgbRotWaitComplete();

#endif
