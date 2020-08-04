/*! \file timer.h
    \brief Timer
 */

#ifndef __ORCUS_TIMER_H__
#define __ORCUS_TIMER_H__

#include <stdint.h>

// timer increments once every 0.135uS and overflows at 0xFFFFFFFF then carries on counting
/**
   @def TIMER_NS_PER_TICK
   @brief Nanoseconds per timer tick.

   Nanoseconds per timer tick. Timer ticks once every 0.135uS.
 */
#define TIMER_NS_PER_TICK 135

/**
   @brief Get current timer tick.

   Get current timer tick.

   Timer ticks every 0.135uS and overflows at 0xFFFFFFFF then carries on counting.

   @return Current timer tick
 */
extern uint32_t timerGet();

/**
   @brief Reset timer to count from specific tick.

   Reset timer to count from specific tick.

   @note There is a bug in the MMSP2 silicon which prevents starting the timer from 0, so this function will start it from 1 if you try to pass 0.

   @param count Tick to start counting from
   @return Tick the timer was at before being reset
 */
extern uint32_t timerSet(uint32_t count);

/**
   @brief Wait for a given number of nanoseconds.

   Wait for a given number of nanoseconds.

   @note This function won't work correctly if you try to wait for longer than 579.82s due to timer overflow.

   @param ns Nanoseconds to wait
 */
extern void timerSleepNs(unsigned long ns);

/**
   @brief Calculate nanoseconds elapsed since a given tick.

   Calculate nanoseconds elapsed since a given tick. Optionally store the current tick.

   Useful for timeouts, or calculating time between frames.

   @note This function won't work correctly for periods longer than 579.82s due to timer overflow.
 */
extern unsigned long timerNsSince(uint32_t lastTick, uint32_t* storeCurrent);

#endif
