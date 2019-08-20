#include <stdlib.h>
#include <gp2xregs.h>
#include <orcus.h>

int main() {
  orcus_init();

  void* a = malloc(100);
  void* b = malloc(100);
  orcus_uart_printf("first malloc at: 0x%08x\r\n", a);
  orcus_uart_printf("second malloc at: 0x%08x\r\n", b);
  
  orcus_uart_printf("Adan is %d\r\n", 103);
  orcus_uart_printf("Bob is %d\r\n", 583);

  void* fb = malloc(320*240*2);
  orcus_uart_printf("Allocated framebuffer at %p\r\n", fb);

  rgbSetPixelFormat(RGB565);
  orcus_uart_printf("Pixel format is now RGB565\r\n");
  
  rgbRegionNoBlend(REGION1);
  orcus_uart_printf("No blending REGION1\r\n");

  rgbSetRegionPosition(REGION1, 0, 0, 320, 240);
  orcus_uart_printf("Set REGION1 position\r\n");

  rgbSetFbAddress(fb);
  orcus_uart_printf("Set FB address\r\n");

  rgbToggleRegion(REGION1, true);
  orcus_uart_printf("Enable RGB REGION1\r\n");

  uint16_t* fb16 = (uint16_t*) fb;
  orcus_uart_printf("Preparing to write to FB: %p\r\n", fb16);
  
  for(int y = 0 ; y < 240 ; y++) {
    for(int x = 0 ; x < 320 ; x++) {
      uint16_t* newLoc = (fb16+((y*320)+x));
      //      orcus_uart_printf("Setting (%d, %d): %p\r\n", x, y, newLoc);
      *newLoc = 0x1FE0;//0xF800;
    }
  }


  
  while(1);
}
