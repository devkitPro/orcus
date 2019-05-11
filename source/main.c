#include <stdlib.h>
#include <gp2xregs.h>
#include <orcus.h>

void init() {
  // a little test to see if we can even get the thing to execute, if we can read 0xdeadbeef
  // from 0x2000 in memory via jtag, the binary is running
  volatile r32 test = (r32) 0x2000;
  *test = 0xdeadbeee;

  unsigned int test2_data = 0x123;
  volatile r32 test2 = (r32) 0x3000;
  *test2 = test2_data;

  unsigned int test3_data;
  volatile r32 test3 = (r32) 0x4000;
  *test3 = test3_data;

  //   volatile r16 test4 = (r16) 0xc0001200;
  //    *test4 = 0x2;
  //        REGISTER(adanstest, 0x1200, 16);
  //      *adanstest = 0x1;
  //  *RR(0x1200, 16) = 0x3;
  //  REG16(0x1200) = 0x2;
}

int main() {
  orcus_init();

  init();
    while(1) {
          orcus_uart_putc('A', true);
    orcus_uart_putc('d', true);
    orcus_uart_putc('a', true);
    orcus_uart_putc('n', true);
    orcus_uart_putc('\r', true);    
    orcus_uart_putc('\n', true);    

  }
}
