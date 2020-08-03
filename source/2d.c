#include <gp2xregs.h>
#include <orcus.h>
#include <2d.h>
#include <stdlib.h>

#define FRAC_16BPP(x) ((x%2)*16)
#define FRAC_8BPP(x) ((x%4)*8)
#define FRAC_1BPP(x) (x%32)
#define FRAC(bpp, x) ( \
  bpp == P8BPP ? FRAC_8BPP(x) : \
  bpp == RGB565 ? FRAC_16BPP(x) : \
  FRAC_1BPP(x) \
)

static uint16_t transparencyColour = 0xF81F;

volatile uint32_t* pattern = (r32) (((uint32_t)&__io_base)+0x20000000+PAT);

void rgbBlit(Graphic* src, Rect* srcRect, Graphic* dest, int x, int y, bool enableTransparency) {
  rgbRasterOp(src, srcRect, dest, &((Rect){x, y, srcRect->w, srcRect->h}), ROP_SRCCOPY, NULL, enableTransparency, 0, 0);
}

void rgbBlit1bpp(Graphic* src, Rect* srcRect, Graphic* dest, int x, int y, bool enableTransparency, uint16_t fgCol, uint16_t bgCol) {
  rgbRasterOp(src, srcRect, dest, &((Rect){x, y, srcRect->w, srcRect->h}), ROP_SRCCOPY, NULL, enableTransparency, fgCol, bgCol);
}

void rgbSolidFill(Graphic* dest, Rect* region, uint16_t colour) {
  rgbRasterOp(NULL, NULL, dest, region, ROP_PATCOPY, &((RasterPattern){colour, colour, B1BPP, 0}), false, 0, 0);
}

void rgbPatternFill(Graphic* dest, Rect* region, RasterPattern* pattern, bool enableTransparency) {
  rgbRasterOp(NULL, NULL, dest, region, ROP_PATCOPY, pattern, enableTransparency, 0, 0);
}

void rgbRasterOp(Graphic* src, Rect* srcRect, Graphic* dest, Rect* destRect, uint8_t rasterOp, RasterPattern* pattern, bool enableTransparency, uint16_t srcFgCol, uint16_t srcBgCol) {
  int destBpp = dest->format == P8BPP ? 1 : 2;
  unsigned int destStride = dest->w*destBpp;
  uint32_t destAddress = ((uint32_t)((destRect->y*destStride+(destRect->x*destBpp)) + ((uint8_t*)dest->data))) & ~0x3;

  FREG32(DSTCTRL) = (dest->format == P8BPP ? 0 : BIT(5))
    | FRAC(dest->format, destRect->x);
  FREG32(DSTADDR) = destAddress;
  FREG32(DSTSTRIDE) = destStride;

  if(src == NULL) {
    FREG32(SRCCTRL) = 0x0;
    FREG32(SRCADDR) = 0x0;
    FREG32(SRCSTRIDE) = 0x0;
  } else {
    int sourceBpp = src->format == RGB565 ? 2 : 1;
    unsigned int sourceStride = src->format == B1BPP ? 1 : src->w*sourceBpp;
    uint32_t sourceAddress = ((uint32_t)((srcRect->y*sourceStride+(srcRect->x*sourceBpp)) + ((uint8_t*)src->data))) & ~0x3;

    FREG32(SRCFORCOLOR) = src->format == B1BPP ? srcFgCol : 0;
    FREG32(SRCBACKCOLOR) = src->format == B1BPP ? srcBgCol : 0;
    
    FREG32(SRCCTRL) = BIT(8)
      | BIT(7)
      | ((src->format == P8BPP ? 0 : src->format == RGB565 ? 1 : 2) << 5)
      | FRAC(src->format, srcRect->x);
    FREG32(SRCADDR) = sourceAddress;
    FREG32(SRCSTRIDE) = sourceStride;
  }

  unsigned int szX = destRect->w & 0x7FF;
  unsigned int szY = destRect->h & 0x7FF;
  FREG32(SIZE) = (szY<<16) | szX;

  if(pattern == NULL) {
    FREG32(PATCTRL) = 0x0;
  } else {
    FREG32(PATCTRL) = (pattern->format == B1BPP ? 0 : BIT(6))
      | BIT(5) // if you passed a pattern we can assume we want to enable it
      | ((pattern->format == P8BPP ? 0 :
	 pattern->format == RGB565 ? 1 :
	 pattern->format == B1BPP ? 2 : 3) << 3)
      | pattern->yOffset;
    FREG32(PATFORCOLOR) = pattern->fgCol;
    FREG32(PATBACKCOLOR) = pattern->bgCol;
  }

  FREG32(CTRL) = (((uint32_t)transparencyColour) << 16)
    | ((dest->format == RGB565 && enableTransparency) ? BIT(11) : 0)
    | BIT(10)
    | BIT(9)
    | BIT(8)
    | rasterOp;
}

// only valid when dest is rgb565
void rgbSetTransparencyColour(uint16_t colour) {
  transparencyColour = colour;
}

void rgbRasterRun() {
  FREG32(RUN) |= BIT(0);
}

bool rgbRasterIsRunning() {
  return FREG32(RUN) & BIT(0);
}

void rgbRasterWaitComplete() {
  while(rgbRasterIsRunning());
}

// x,y is top left corner of destination
void rgbRotBlit(Graphic* src, Rect* srcRect, Graphic* dest, int x, int y, Angle angle) {
  int sourceBpp = src->format == P8BPP ? 0 :
    src->format == RGB565 ? 2 :
    src->format == RGB888 ? 1 : 3; // TODO - test that RGB888 = 1, the manual appears to have it swapped with RGB565
  int sourceStride = src->w*sourceBpp;
  int destBpp = dest->format == P8BPP ? 0 :
    dest->format == RGB565 ? 2 :
    dest->format == RGB888 ? 1 : 3;
  int destStride = dest->w*destBpp;
  FREG32(ROT_DSTSTRIDE) = destStride;
  FREG32(ROT_SRCSTRIDE) = sourceStride;
  FREG32(ROT_PICSIZE) = (srcRect->h << 16) | srcRect->w;
  
  FREG32(ROT_DSTADDR) = ((uint32_t)((y*destStride+(x*destBpp)) + ((uint8_t*)dest->data)));

  int posX;
  int posY;
  if(angle == DEG0) {
    posX = srcRect->x;
    posY = srcRect->y;
  } else if(angle == DEG90) {
    posX = srcRect->x;
    posY = srcRect->y + srcRect->h - 1;
  } else if(angle == DEG180) {
    posX = srcRect->x + srcRect->w - 1;
    posY = srcRect->y + srcRect->h - 1;
  } else {
    posX = srcRect->x + srcRect->w - 1;
    posY = srcRect->y;
  }
  FREG32(ROT_SRCADDR) = ((uint32_t)(((sourceStride*posY) + (posX*sourceBpp) + ((uint8_t*)src->data))));

  FREG32(ROT_CNTL) = (angle << 3)
    | ((src->format == P8BPP ? 0 :
       src->format == RGB565 ? 1 :
       src->format == RGB888 ? 2 : 3) << 1);
}

void rgbRotRun() {
  FREG32(ROT_CNTL) |= BIT(0);
}

bool rgbRotIsRunning() {
  return FREG32(ROT_CNTL) & BIT(0);
}

void rgbRotWaitComplete() {
  while(rgbRotIsRunning());
}
