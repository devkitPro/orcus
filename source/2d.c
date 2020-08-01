#include <gp2xregs.h>
#include <orcus.h>
#include <2d.h>

#define FRAC_16BPP(x) ((x%2)*16)
#define FRAC_8BPP(x) ((x%4)*8)
#define FRAC_1BPP(x) (x%32)
#define FRAC(bpp, x) ( \
  bpp == P8BPP ? FRAC_8BPP(x) : \
  bpp == RGB565 ? FRAC_16BPP(x) : \
  FRAC_1BPP(x) \
)

static uint16_t transparencyColour = 0xF81F;

void rgbBlit(Graphic* src, Rect* srcRect, Graphic* dest, int x, int y, bool enableTransparency) {
  int sourceBpp = src->format == P8BPP ? 1 : 2;
  unsigned int sourceStride = src->w*sourceBpp;
  uint32_t sourceAddress = ((uint32_t)((srcRect->y*sourceStride+(srcRect->x*sourceBpp)) + ((uint8_t*)src->data))) & ~0x3;

  int destBpp = dest->format == P8BPP ? 1 : 2;
  unsigned int destStride = dest->w*destBpp;
  uint32_t destAddress = ((uint32_t)((y*destStride+(x*destBpp)) + ((uint8_t*)dest->data))) & ~0x3;

  uartPrintf("sourceAddr: %p, destAddr: %p\n", sourceAddress, destAddress);
  uartPrintf("sourceStride: %u, destStride: %u\n", sourceStride, destStride);
  uartPrintf("sourceBpp: %d, destBpp: %d\n", sourceBpp, destBpp);
  uartPrintf("sourceFrac: %u, destFrac: %u\n", FRAC(src->format, x), FRAC(dest->format, x));

  uint32_t dstctrl = (dest->format == P8BPP ? 0 : BIT(5))
    | FRAC(dest->format, x);
  FREG32(DSTCTRL) = dstctrl;
  uartPrintf("DSTCTRL: 0x%x\n", dstctrl);
  FREG32(DSTADDR) = destAddress;
  FREG32(DSTSTRIDE) = destStride;

  uint32_t srcctrl = BIT(8)
    | BIT(7) // TODO - is this correct?
    | ((src->format == P8BPP ? 0 : src->format == RGB565 ? 1 : 2) << 5)
    | FRAC(src->format, srcRect->x);
  FREG32(SRCCTRL) = srcctrl;
  uartPrintf("SRCCTRL: 0x%x\n", srcctrl);
  FREG32(SRCADDR) = sourceAddress;
  FREG32(SRCSTRIDE) = sourceStride;

  unsigned int szX = (srcRect->w+x >= dest->w ? (dest->w - x) : srcRect->w) & 0x7FF;
  unsigned int szY = (srcRect->h+y >= dest->h ? (dest->h - y) : srcRect->h) & 0x7FF;
  uartPrintf("szX: %d, szY: %d\n", szX, szY);
  FREG32(SIZE) = (szY<<16) | szX;

  volatile uint32_t ctrl =
     (dest->format == RGB565 && enableTransparency) ? BIT(11) : 0
    | BIT(10)
    | BIT(9)
    | BIT(8)
    | ROP_SRCCOPY
    | (((uint32_t)transparencyColour) << 16);
  uartPrintf("CTRL: 0x%x, rop: 0x%x, thing: 0x%x\n", ctrl, ROP_SRCCOPY);
  FREG32(CTRL) = ctrl;
}

void rgbSet1bppFgColour(uint16_t colour) {
  FREG32(SRCFORCOLOR) = colour;
}

void rgbSet1bppBgColour(uint16_t colour) {
  FREG32(SRCBACKCOLOR) = colour;
}

// only valid when dest is rgb565
void rgbSetTransparencyColour(uint16_t colour) {
  transparencyColour = colour;
}

void rgb2dRun() {
  FREG32(RUN) |= BIT(0);
}

bool rgb2dIsRunning() {
  return FREG32(RUN) & BIT(0);
}

void rgb2dWaitComplete() {
  while(rgb2dIsRunning());
}

// TODO - stuff involving pattern

void rgbRotBlit(Graphic* src, Rect* srcRect, Graphic* dest, int x, int y, Angle angle) {
}
