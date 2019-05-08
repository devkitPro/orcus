#include <gp2xregs.h>

void init() {
  // a little test to see if we can even get the thing to execute, if we can read 0xdeadbeef
  // from 0x2000 in memory via jtag, the binary is running
  volatile r32 test = (r32) 0x2000;
  *test = 0xdeadbeea;

  unsigned int test2_data = 0x123;
  volatile r32 test2 = (r32) 0x3000;
  *test2 = test2_data;

  unsigned int test3_data;
  volatile r32 test3 = (r32) 0x4000;
  *test3 = test3_data;
  
}

int main() {
  init();
  while(1);
}
