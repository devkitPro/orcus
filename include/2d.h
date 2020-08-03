#ifndef __2D_H__
#define __2D_H__

#include <orcus.h>

typedef enum {
	      DEG0 = 0,
	      DEG90 = 1,
	      DEG180 = 2,
	      DEG270 = 3
} Angle;

typedef struct {
  const void* data;
  int w;
  int h;
  RgbFormat format;
} Graphic;

typedef struct {
  int x;
  int y;
  int w;
  int h;
} Rect;

typedef struct {
  uint16_t fgCol;
  uint16_t bgCol;
  RgbFormat format; // only 8bpp, 16bpp or 1bpp are valid
  int yOffset;
} RasterPattern;

#define ROP_SRCCOPY 0xCC // blit
#define ROP_PATCOPY 0xF0 // fill

extern volatile uint32_t* pattern;

extern void rgbBlit(Graphic* src, Rect* sourceRect, Graphic* dest, int x, int y, bool enableTransparency);
extern void rgbRasterOp(Graphic* src, Rect* srcRect, Graphic* dest, Rect* destRect, uint8_t rasterOp, RasterPattern* pattern, bool enableTransparency, uint16_t srcFgCol, uint16_t srcBgCol);
extern void rgb2dRun();
extern bool rgb2dIsRunning();
extern void rgb2dWaitComplete();
extern void rgbSetTransparencyColour(uint16_t colour);

#endif
