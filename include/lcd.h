/*! \file lcd.h
    \brief LCD
 */

#ifndef __ORCUS_LCD_H__
#define __ORCUS_LCD_H__

#include <stdint.h>

/**
   @brief Check if LCD is currently in vsync period.

   Check if LCD is currently in vsync period.

   @return true if LCD is currently in vsync period, otherwise false
 */
extern bool lcdVSync();

/**
   @brief Wait until LCD is in next vsync period.

   Wait until LCD is in next vsync period. If LCD is currently in vsync when this is called, it will wait until it has ended, and then again until the next one begins.
 */
extern void lcdWaitNextVSync();

/**
   @brief Check if LCD is currently in hsync period.

   Check if LCD is currently in hsync period.

   @return true if LCD is currently in hsync period, otherwise false
 */
extern bool lcdHSync();

/**
   @brief Wait until LCD is in next hsync period.

   Wait until LCD is in next hsync period. If LCD is currently in hsync when this is called, it will wait until it has ended, and then again until the next one begins.
 */
extern void lcdWaitNextHSync();

#endif
