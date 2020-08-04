/*! \file uart.h
    \brief UART
 */

#ifndef __ORCUS_UART_H__
#define __ORCUS_UART_H__

#include <stdint.h>

/**
   UART parity settings.
 */
typedef enum {
	      NONE /** No parity bit */,
	      ODD /** Odd parity bit */,
	      EVEN /** Even parity bit */
} Parity;

/**
   @brief Configure UART parameters.

   Configures UART parameters.

   Example (115200 8N1):
   @code
   uartConfigure(115200, 8, NONE, 1)
   @endcode
 */
extern void uartConfigure(int baudRate, int bitsPerFrame, Parity parity, int stopBits);

/**
   @brief Output character over UART.

   Outputs a single character over UART.

   @param c Character to output
   @param isBlocking If true will wait until the output buffer has space for this character, will fail fast if false
   @returns The character sent if successful. If non-blocking operation is used and the buffer is full, -1.
 */
extern char uartPutc(char c, bool isBlocking);

/**
   @brief Get a character from UART.

   Gets a single character from UART.

   @param isBlocking If true, will wait for a character to become available, otherwise will return immediately if no character is in the input buffer.
   @return The next character from the input buffer. If non-blocking operation is used and the buffer is empty, EOF.
 */
extern int uartGetc(bool isBlocking);

/**
   @brief printf over UART.

   As per the standard printf, with output directly to UART.

   @note This has a maximum output buffer of 256 bytes so you may be better off using the newlib printf.
 */
extern void uartPrintf(const char* format, ...);

/**
   @brief Set UART echoing.

   If enabled, uartGetc will echo any character received back out to the UART. This is handy when implementing a command line interface over UART.

   @param isEnabled If true, UART echoing will be enabled.
 */
extern void uartSetEcho(bool isEnabled);

#endif
