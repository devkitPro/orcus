#ifndef __ORCUS_H__
#define __ORCUS_H__

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t uint4_t;

// memory layout from linker and init function
extern uint32_t __heap_start;
extern uint32_t __heap_end;

// orcus system functions
extern void orcus_init();
extern void orcus_init_syscalls();
extern void orcus_configure_peripherals();
extern int orcus_calculate_uart_diviser(int baudRate);
extern void orcus_set_ram_timings(int tRC, int tRAS, int tWR, int tMRD, int tRFC, int tRP, int tRCD);
extern void orcus_default_ram_timings();
extern void orcus_fast_ram_timings();

typedef enum { NONE, ODD, EVEN } Parity;
extern void orcus_configure_uart(int baudRate, int bitsPerFrame, Parity parity, int stopBits);
extern char orcus_uart_putc(char c, bool isBlocking);
extern char orcus_uart_getc(bool isBlocking);
extern void orcus_uart_printf(const char* format, ...);

extern void orcus_configure_gpio();

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

extern void orcus_configure_display();
extern void orcus_set_rgb_format(RgbFormat format);
extern void orcus_rgb_toggle_region(RgbRegion region, bool onOff);
extern void orcus_rgb_region_alpha(RgbRegion region, uint4_t alpha);
extern void orcus_rgb_region_colourkey(RgbRegion region);
extern void orcus_rgb_set_colourkey(uint8_t r, uint8_t g, uint8_t b);
extern void orcus_rgb_region_noblend(RgbRegion region);
extern void orcus_rgb_scale(int srcW, int srcH);
extern void orcus_set_rgb_address(void* fb);

#endif
