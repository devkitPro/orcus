#include <stdio.h>
#include <stdarg.h>
#include <gp2xregs.h>
#include <orcus.h>

static int orcus_calculate_uart_baud(int baudRate) {
    return ((14745600/(baudRate*16))-1);
}

static int orcus_calculate_uart_diviser(int baudRate) {
    const int clkBasis = 18432000;
    return (int) (clkBasis / (baudRate * 16)) - 1;
}

void uartConfigure(int baudRate, int bitsPerFrame, Parity parity, int stopBits) {
  REG16(URT0CSETREG) = URT0CSETREG_UART0(URTnCSETREG_CLKSRC(APLL_CLK) | URTnCSETREG_CLKDIV(orcus_calculate_uart_diviser(baudRate)));
  REG16(LCON0) = LCONx_SIR_MODE(LCONx_SIR_MODE_NORMAL)
    | LCONx_PARITY(parity == ODD ? LCONx_PARITY_ODD :
		   parity == EVEN ? LCONx_PARITY_EVEN : LCONx_PARITY_NONE)
    | LCONx_STOPBIT(stopBits == 2 ? LCONx_STOPBIT_TWO : LCONx_STOPBIT_ONE)
    | LCONx_WORD_LEN(stopBits == 5 ? LCONx_WORD_LEN_5BITS :
		     stopBits == 6 ? LCONx_WORD_LEN_6BITS :
		     stopBits == 7 ? LCONx_WORD_LEN_7BITS : LCONx_WORD_LEN_8BITS);
  REG16(UCON0) = UCONx_TRANS_MODE(UCONx_MODE_INTPOLL) | UCONx_RECEIVE_MODE(UCONx_MODE_INTPOLL);
  REG16(FCON0) = FCONx_FIFO_EN | FCONx_TX_FIFO_RESET | FCONx_RX_FIFO_RESET;
  REG16(BRD0) = orcus_calculate_uart_baud(baudRate);
}

char uartPutc(char c, bool isBlocking) {
  if(isBlocking) {
    while(REG16(FSTATUS0)&FSTATUSx_TX_FIFO_FULL);
    return REG8(THB0) = c;
  } else {
    return REG16(FSTATUS0)&FSTATUSx_TX_FIFO_FULL ? -1 : (REG8(THB0) = c);
  }
}

char uartGetc(bool isBlocking) {
  if(isBlocking) {
    while(FSTATUSx_RX_FIFO_COUNT(FSTATUS0) == 0);
    return REG8(RHB0);
  } else {
    return FSTATUSx_RX_FIFO_COUNT(FSTATUS0) == 0 ? -1 : REG8(RHB0);
  }
}

void uartPrintf(const char* format, ...) {
  const int bufferSize = 256;
  char buffer[bufferSize];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end (args);

  for(int i = 0 ; i < bufferSize ; i++) {
    if(buffer[i] == '\0') return;
    else uartPutc(buffer[i], true);
  }
}
