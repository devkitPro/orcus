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

#define ROP_SRCCOPY 0xCC

extern void rgbBlit(Graphic* src, Rect* sourceRect, Graphic* dest, int x, int y, bool enableTransparency);
extern void rgb2dRun();
extern bool rgb2dIsRunning();
extern void rgb2dWaitComplete();
extern void rgbSetTransparencyColour(uint16_t colour);

#endif
