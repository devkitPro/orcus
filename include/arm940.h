/*! \file arm940.h
    \brief ARM940
 */

#ifndef __ORCUS_ARM940_H__
#define __ORCUS_ARM940_H__

#include <stdint.h>

/**
   @brief Initialise ARM940T.

   Initialise the ARM940T second CPU and configure the base address (in 16M increments) in physical memory which will appear as 0x0 from the perspective of the ARM940T.

   This leaves the ARM940T in RESET state, and enables the clock.

   Valid base addresses are 16M, 32M and 48M (banks 1, 2 and 3 respectively) to prevent overlapping with the ARM920T vector table (always stored at 0x0 in physical memory). It is possible to set both CPUs to start at 0x0 physical but in this case they will share a RESET vector and will execute the same code so it is highly discouraged unless you have an unusual use case in mind, and is not supported with this function.

   Further documentation can be found on the Orcus wiki regarding using the ARM940T.

   @note This should only be called from the ARM920T.

   @param memoryBank Memory bank to use as start of ARM940T address range (1 - 3)
   @return 0 if successful, non-zero otherwise
 */
extern int arm940Init(int memoryBank);

/**
   @brief Check if the ARM940T is running.

   Check if the ARM940T is running.

   @note It makes no sense to call this from the ARM940T.

   @return true if the ARM940T is running, false if it is being held in RESET state
 */
extern bool arm940Running();

/**
   @brief Halt the ARM940T.

   Halt the ARM940T and hold it in RESET state.

   @note You can call this from the ARM940T but you won't get control back afterwards.
 */
extern void arm940Halt();

/**
   @brief Start execution on the ARM940T.

   Start execution on the ARM940T from its RESET vector entry (0x0) by releasing it from RESET state.

   @note It makes no sense to call this from the ARM940T.
 */
extern void arm940Run();

/**
  @brief Resume execution on the ARM940T.

  Enable the clock on the ARM940T to resume execution at the same point it was at when the clock was disabled.

  @note It makes no sense to call this from the ARM940T.

  @see arm940ClockOff
 */
extern void arm940ClockOn();

/**
   @brief Pause execution on the ARM940T.

   Pause execution on the ARM940T by disabling its clock.

   @note You can call this from the ARM940T but you won't get control back until the ARM920 calls arm940ClockOn()

   @see arm940ClockOn
 */
extern void arm940ClockOff();

/**
   @brief Check if the ARM940T has a clock signal.

   Check if the ARM940T has a clock signal, to see if execution is paused.

   @return true if the ARM940T has a clock signal, false otherwise (i.e. it is paused)
 */
extern bool arm940HasClock();

/**
   @brief Check if executing on the ARM940T.

   Check if this call is being executed on the ARM940T.

   @return true if this is called from the ARM940T, false if called from the ARM920T
 */
extern bool arm940IsThis();

/**
   @brief Import a pointer from the other CPU.

   Import a pointer from the other CPU into the address space of the CPU we are running on.

   @warning It may not be possible to import a pointer from memory which is only viewable from the ARM920T when on the ARM940T. It is always possible to import from the ARM940T to the ARM920T however.

   @param addr Address to import from other CPU
   @return Pointer to the given address within the address space of the current CPU
 */
extern void* importPointer(uint32_t addr);

/**
   @brief Export a pointer to the other CPU.

   Export a pointer to the address space of the other CPU.

   @warning It may not be possible to export a pointer from memory which is only viewable from the ARM920T to the ARM940T. It is always possible to export from the ARM940T to the ARM920T however.

   @param ptr Pointer to export
   @return Address of ptr within the address space of the other CPU
 */
extern uint32_t exportPointer(void* ptr);

/**
   @var arm940Data
   @brief Pointer to start of 16x 16-bit shared data registers.

   Pointer to start of 16x 16-bit shared data registers. These can be written to from either CPU, but can only trigger an interrupt on the ARM940T.
 */
extern volatile uint16_t* arm940Data;
/**
   @var arm920Data
   @brief Pointer to start of 16x 16-bit shared data registers.

   Pointer to start of 16x 16-bit shared data registers. These can be written to from either CPU, but can only trigger an interrupt on the ARM920T.
 */
extern volatile uint16_t* arm920Data;

#endif
