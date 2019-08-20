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

  uint16_t* fb0 = malloc(320*240*2);
  orcus_uart_printf("Allocated framebuffer 0 at %p\r\n", fb0);  
  uint16_t* fb1 = malloc(320*240*2);
  orcus_uart_printf("Allocated framebuffer 1 at %p\r\n", fb1);

  rgbSetPixelFormat(RGB565);
  orcus_uart_printf("Pixel format is now RGB565\r\n");
  
  rgbRegionNoBlend(REGION1);
  orcus_uart_printf("No blending REGION1\r\n");

  rgbSetRegionPosition(REGION1, 0, 0, 320, 240);
  orcus_uart_printf("Set REGION1 position\r\n");

  rgbSetFbAddress((void*)fb0);
  orcus_uart_printf("Set FB address\r\n");

  rgbToggleRegion(REGION1, true);
  orcus_uart_printf("Enable RGB REGION1\r\n");

  for(int i = 320*240 ; i-- ; )
    *(fb0+i) = 0x1FE0;
  for(int i = 320*240 ; i-- ; )
    *(fb1+i) = 0xF800;

  while(1) {
    if(REG16(GPIOCPINLVL) & (1<<8)) rgbSetFbAddress(fb0);
    else rgbSetFbAddress(fb1);
  }
}
