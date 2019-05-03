#include <gp2xregs.h>

int main() {
  *_irq = 0xdeadbeef;

  while(1);
}
