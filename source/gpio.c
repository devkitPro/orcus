#include <gp2xregs.h>
#include <orcus.h>
#include <sys/unistd.h>
#include <time.h>

extern void orcus_delay(int loops);

bool orcus_configure_gpio() {
  // lcd
  REG16(GPIOBALTFNHI) = (ALTFN1 << 14)
    | (ALTFN1 << 12)
    | (ALTFN1 << 10)
    | (ALTFN1 << 8)
    | (ALTFN1 << 6)
    | (ALTFN1 << 4)
    | (ALTFN1<< 2)
    | (ALTFN1<< 0);
  REG16(GPIOBALTFNLOW) = 0xAA50;
  REG16(GPIOAALTFNLOW) = (ALTFN1 << 14)
    | (ALTFN1 << 12)
    | (ALTFN1 << 10)
    | (ALTFN1 << 8)
    | (ALTFN1 << 6)
    | (ALTFN1 << 4)
    | (ALTFN1 << 2)
    | (ALTFN1 << 0);
  REG16(GPIOAALTFNHI) = (ALTFN1 << 14)
    | (ALTFN1 << 12)
    | (ALTFN1 << 10)
    | (ALTFN1 << 8)
    | (ALTFN1 << 6)
    | (ALTFN1 << 4)
    | (ALTFN1 << 2)
    | (ALTFN1 << 0);

  REG16(GPIOBPUENB) = REG16(GPIOBPUENB) & ~0xF0;

  // set up memc so we can detect net2272 in F100
  REG16(GPIOGALTFNLOW) = REG16(GPIOGALTFNLOW)
    | (ALTFN1 << 10)
    | (ALTFN1 << 8)
    | (ALTFN1 << 6)
    | (ALTFN1 << 4)
    | (ALTFN1 << 2);

  REG16(GPIOEALTFNLOW) = (ALTFN1 << 14)
    | (ALTFN1 << 12)
    | (ALTFN1 << 10)
    | (ALTFN1 << 8)
    | (ALTFN1 << 6)
    | (ALTFN1 << 4)
    | (ALTFN1 << 2)
    | ALTFN1;
  REG16(GPIOEALTFNHI) = (ALTFN1 << 14)
    | (ALTFN1 << 12)
    | (ALTFN1 << 10)
    | (ALTFN1 << 8)
    | (ALTFN1 << 6)
    | (ALTFN1 << 4)
    | (ALTFN1 << 2)
    | ALTFN1;
  REG16(GPIOIALTFNHI) = (REG16(GPIOIALTFNHI) & ~(0x3 << 4)) | (ALTFN1 << 4);

  REG16(GPIOJALTFNHI) = (REG16(GPIOJALTFNHI) & ~(0x3 << 6)) | (ALTFN1 << 6);
  REG16(GPIOJALTFNHI) = (REG16(GPIOJALTFNHI) & ~(0x3 << 8)) | (ALTFN1 << 8);
  REG16(GPIOKALTFNLOW) = 0x0808;
  REG16(GPIODALTFNLOW) = (REG16(GPIODALTFNLOW) & ~(0x3 << 8)) | (OUT << 8); // 0x29aa
  // end net2272

  bool isF200 = gp2xIsF200();
  
  // set GPIO pins for LCD
  if(isF200) {
    REG16(GPIOLALTFNHI) = (OUT << 6)
      | (ALTFN1 << 4)
      | (ALTFN1 << 2)
      | (ALTFN1 << 0)
      | (REG16(GPIOLALTFNHI) & 0xFF00);
  } else {
    REG16(GPIOLALTFNHI) = (IN << 6)
      | (ALTFN1 << 4)
      | (ALTFN1 << 2)
      | (ALTFN1 << 0)
      | (REG16(GPIOLALTFNHI) & 0xFF00);
  }
  // set GPIO L pins to SD mode
  REG16(GPIOLALTFNLOW) = (ALTFN1 << 14)
    | (ALTFN1 << 12)
    | (ALTFN1 << 10)
    | (ALTFN1 << 8)
    | (ALTFN1 << 6)
    | (ALTFN1 << 4)
    | (ALTFN1 << 2)
    | (ALTFN1 << 0);
  REG16(GPIOLPUENB) |= 0x003F;
  // set I14 to in, with pull up in order to detect SD card
  REG16(GPIOIALTFNHI) = (REG16(GPIOIALTFNHI)&~(0x3 << 12)) | (IN << 12);
  REG16(GPIOIPUENB) = REG16(GPIOIPUENB) & 0xBFFF;

  REG16(GPIOHALTFNLOW) = REG16(GPIOHALTFNLOW)
    | (OUT << 8)
    | (OUT << 6)
    | (OUT << 4)
    | (OUT << 2)
    | (OUT << 0);

  if(isF200) {
    REG16(GPIOFALTFNLOW) = (ALTFN1 << 14)
      | (OUT << 12)
      | (IN << 10)
      | (IN << 8)
      | (OUT << 6)
      | (OUT << 4)
      | (OUT << 2)
      | OUT;
  }

  REG16(GPIOBOUT) = (REG16(GPIOBOUT) & ~0x8) | CX25874_SLEEP;
  REG16(GPIOHOUT) = (REG16(GPIOHOUT) & ~0xF) | BIT(3) | BATTERY_LED;
  orcus_delay(10);
  REG16(GPIOHOUT) = REG16(GPIOHOUT) | LCD_VGH;
  orcus_delay(10);

  if(!isF200) { // TODO - should these not really be in display.c?
    REG16(GPIOBOUT) |= LCD_RESET;
    REG16(GPIOHOUT) |= BACKLIGHT_F100;
  }

  return isF200;
}
