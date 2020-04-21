#include <stdlib.h>
#include <gp2xregs.h>
#include <orcus.h>
#include <fat.h>
#include <sys/dir.h>
#include <dirent.h>
#include <unistd.h>

int main() {
  gp2xInit();

  uart_printf("\r\n\n\n**********************************\r\n* Welcome to Orcus test program  *\r\n**********************************\r\n\r\n");

  void* a = malloc(100);
  void* b = malloc(100);
  uart_printf("first malloc at: 0x%08x\r\n", a);
  uart_printf("second malloc at: 0x%08x\r\n", b);
  
  uart_printf("Adan is %d\r\n", 103);
  uart_printf("Bob is %d\r\n", 583);

  uint16_t* fb0 = malloc(320*240*2);
  uart_printf("Allocated framebuffer 0 at %p\r\n", fb0);  
  uint16_t* fb1 = malloc(320*240*2);
  uart_printf("Allocated framebuffer 1 at %p\r\n", fb1);

  rgbSetPixelFormat(RGB565);
  uart_printf("Pixel format is now RGB565\r\n");
  
  rgbRegionNoBlend(REGION1);
  uart_printf("No blending REGION1\r\n");

  rgbSetRegionPosition(REGION1, 0, 0, 320, 240);
  uart_printf("Set REGION1 position\r\n");

  rgbSetFbAddress((void*)fb0);
  uart_printf("Set FB address\r\n");

  rgbToggleRegion(REGION1, true);
  uart_printf("Enable RGB REGION1\r\n");

  uart_printf("Waiting for 5 seconds\r\n");
  usleep(5000000);
  uart_printf("Finished waiting\r\n");
  
  for(int i = 320*240 ; i-- ; )
    *(fb0+i) = 0x1FE0;
  for(int i = 320*240 ; i-- ; )
    *(fb1+i) = 0xF800;

  uart_printf("Testing for SD card\r\n");

  SdInfo* sdInfo = (SdInfo*) malloc(sizeof(SdInfo));
  sdInit(sdInfo);
  if(sdInfo->isInserted) {
    uart_printf("Detected %dkb SD card\r\n", sdInfo->sizeKb);

    fatInitDefault();
    DIR *dp;
    struct dirent *ep;

    dp = opendir ("sd:/");
    if (dp != NULL)
      {
	while ((ep = readdir(dp)))
	  uart_printf("%s\r\n", ep->d_name);
	(void) closedir (dp);
      }
    else
      uart_printf("Couldn't open the directory\r\n");

  } else {
    uart_printf("No SD card detected\r\n");
  }
  
  while(1) {
    uint32_t btn = btnState();
    /*    if(btn & A) uart_printf("A pressed\r\n");
    if(btn & B) uart_printf("B pressed\r\n");
    if(btn & X) uart_printf("X pressed\r\n");
    if(btn & Y) uart_printf("Y pressed\r\n");
    if(btn & START) uart_printf("START pressed\r\n");
    if(btn & SELECT) uart_printf("SELECT pressed\r\n");
    if(btn & UP) uart_printf("UP pressed\r\n");
    if(btn & DOWN) uart_printf("DOWN pressed\r\n");
    if(btn & LEFT) uart_printf("LEFT pressed\r\n");
    if(btn & RIGHT) uart_printf("RIGHT pressed\r\n");
    if(btn & UP_LEFT) uart_printf("UP_LEFT pressed\r\n");
    if(btn & UP_RIGHT) uart_printf("UP_RIGHT pressed\r\n");
    if(btn & DOWN_LEFT) uart_printf("DOWN_LEFT pressed\r\n");
    if(btn & DOWN_RIGHT) uart_printf("DOWN_RIGHT pressed\r\n");
    if(btn & VOL_UP) uart_printf("VOL_UP pressed\r\n");
    if(btn & VOL_DOWN) uart_printf("VOL_DOWN pressed\r\n");
    if(btn & STICK) uart_printf("STICK pressed\r\n");
    if(btn & L) uart_printf("L pressed\r\n");
    if(btn & R) uart_printf("R pressed\r\n");*/
    
    if(btn & START) rgbSetFbAddress(fb0);
    else rgbSetFbAddress(fb1);
  }
}
