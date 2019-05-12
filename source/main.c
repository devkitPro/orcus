#include <stdlib.h>
#include <gp2xregs.h>
#include <orcus.h>

int main() {
  orcus_init();

  orcus_uart_printf("Adan is %d\r\n", 103);
  orcus_uart_printf("Bob is %d\r\n", 583);
  
  while(1);
}
