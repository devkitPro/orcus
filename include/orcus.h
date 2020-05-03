#ifndef __ORCUS_H__
#define __ORCUS_H__

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t uint4_t;

// memory layout from linker and init function
extern uint32_t __start_of_heap;

// orcus system functions
extern void gp2xInit();
extern int orcus_calculate_uart_diviser(int baudRate);
extern void orcus_set_ram_timings(int tRC, int tRAS, int tWR, int tMRD, int tRFC, int tRP, int tRCD);
extern void orcus_default_ram_timings();
extern void orcus_fast_ram_timings();

typedef enum { NONE, ODD, EVEN } Parity;
extern void uartConfigure(int baudRate, int bitsPerFrame, Parity parity, int stopBits);
extern char uart_putc(char c, bool isBlocking);
extern char uart_getc(bool isBlocking);
extern void uart_printf(const char* format, ...);

extern void orcus_configure_gpio(bool isF200);

/**
 * Delay for <loops> ticks. A tick is of arbitrary length.
 */
extern void orcus_delay(int loops);

// orcus settings
typedef struct {
  // SYSCLKENREG
  bool externalSystemClock: 1;
  bool adc: 1;
  bool pwm: 1;
  bool dma: 1;
  bool timer: 1;
  bool coprocessor: 1;
  bool gpio: 1;
  bool memC: 1;
  bool memA: 1;
  bool arm940t: 1;

  // COMCLKENREG
  bool ssp: 1;
  bool i2c: 1;
  bool uart3: 1;
  bool uart2: 1;
  bool uart1: 1;
  bool uart0: 1;
  bool usbDeviceController: 1;
  bool usbHostController: 1;

  // VGCLKENREG
  bool deblock: 1;
  bool mpeg: 1;
  bool motionEstimation: 1;
  bool refWrite: 1;
  bool rvld: 1;
  bool qpelGmc: 1;
  bool mpMc: 1;
  bool pdf: 1;
  bool huff: 1;
  bool vpGoff: 1;
  bool vpTop: 1;
  bool isp: 1;
  bool grp: 1;
  bool scale: 1;
  bool disp: 1;

  // ASCLKENREG
  bool cdrom: 1;
  bool ide: 1;
  bool memoryStick: 1;
  bool mmc: 1;
  bool spdifOut: 1;
  bool spdifIn: 1;
  bool i2s: 1;
  bool ac97: 1;
} _mmsp2_peripheral_clock_enable;
extern _mmsp2_peripheral_clock_enable mmsp2PeripheralClockEnable;

typedef enum {
	       REGION1 = 1, REGION2 = 2, REGION3 = 3, REGION4 = 4, REGION5 = 5
} RgbRegion;

typedef enum {
		P4BPP = 0, P8BPP = 1, RGB565 = 2, RGB888 = 3
} RgbFormat;

typedef enum {
	      NO_BLENDING = 0, COLOUR_KEY = 1, ALPHA = 2
} BlendingMode;

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

extern void rgbSetPixelFormat(RgbFormat format);
extern void rgbToggleRegion(RgbRegion region, bool onOff);
extern void rgbRegionBlendAlpha(RgbRegion region, uint4_t alpha);
extern void rgbRegionBlendColourKey(RgbRegion region);
extern void rgbColourKey(uint8_t r, uint8_t g, uint8_t b);
extern void rgbRegionNoBlend(RgbRegion region);
extern void rgbSetScale(int srcW, int srcH);
extern void rgbSetFbAddress(void* fb);
extern void rgbSetRegionPosition(RgbRegion region, int x, int y, int width, int height);
extern void rgbSetFont(uint16_t* font, int charWidth, int charHeight);
extern void rgbPutc(uint16_t* fb, int x, int y, uint16_t colour, char c);
extern void rgbPrintf(uint16_t* fb, int x, int y, uint16_t colour, const char* format, ...);
extern bool lcdVSync();
extern void lcdWaitNextVSync();
extern bool lcdHSync();
extern void lcdWaitNextHSync();

extern uint32_t btnState();

typedef struct {
  bool isInserted;
  int sizeKb;
  uint16_t rca;
} SdInfo;

extern int sdInit();
extern int sdReadBlocks(int startBlock, int numberOfBlocks, uint8_t* dest);
extern bool sdIsInserted();
extern int sdSizeKb();

extern bool gp2xIsF200();

// timer increments once every 0.135uS and overflows at 0xFFFFFFFF then carries on counting
extern uint32_t timerGet();
extern uint32_t timerSet(uint32_t count);

extern void nandRead(uint32_t startAdddr, int numberOfBlocks, void* dest);

extern void enableICache();

#endif
