/*! \file orcus.h
    \brief Main header file for Orcus functions
 */

#ifndef __ORCUS_H__
#define __ORCUS_H__

#include <stdint.h>
#include <stdbool.h>

#include <uart.h>
#include <rgb.h>
#include <2d.h>
#include <audio.h>
#include <arm940.h>
#include <sd.h>
#include <lcd.h>
#include <dma.h>
#include <timer.h>

/**
   @brief Initialise GP2X

   Initialise the GP2X hardware and set up system calls. This must be called before using any other functions from Orcus.
 */
extern void gp2xInit();

/**
   @brief Set custom RAM timings.

   Sets RAM timing registers in MMSP2.
 */
extern void gp2xSetRamTimings(int tRC, int tRAS, int tWR, int tMRD, int tRFC, int tRP, int tRCD);

/**
   @brief Set default recommended RAM timings.

   Equivalent to gp2xSetRamTimings(7, 15, 2, 7, 7, 7, 7)
   @see gp2xSetRamTimings
 */
extern void gp2xSetDefaultRamTimings();

/**
   @brief Set faster RAM timings.

   Equivalent to gp2xSetRamTimings(5, 3, 0, 0, 0, 1, 1)
   @see gp2xSetRamTimings
 */
extern void gp2xSetFastRamTimings();

/**
   @brief Set the CPU clock speed.

   Set the CPU clock speed (both ARM920T and ARM940T use the same clock source).
 */
extern void gp2xSetCpuSpeed(unsigned int mhz);

///@{
/** Button */
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
///@}

/**
   @brief Get current button state.

   Get current button state (raw).

   One can check for a particular button state using &.

   For example:

   @code
   (btnState() & START)
   @endcode

   @return Currently pressed buttons OR-ed together.
   @see A
 */
extern uint32_t btnState();

/**
   @brief Get current button state (debounced).

   Get current button state. This function debounces presses to give a reliable indicator.

   One can check for a particular button state using &.

   For example:

   @code
   (btnStateDebounced() & START)
   @endcode

   @return Currently pressed buttons OR-ed together.
   @see A
 */
extern uint32_t btnStateDebounced();

/**
   @brief Check if running on an F200.

   Check if running on an F200. This tests to see if the NET2272 chip (only fitted to F100s) is present.

   @return true if running on an F200, false if running on an F100
 */
extern bool gp2xIsF200();

/**
   @brief Enable icache on ARM920T

   Enable icache on ARM920T. This is enabled by default in gp2xInit.
   @warning This may have unexpected effects if called from the ARM940T
 */
extern void enableICache();

#endif
