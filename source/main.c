#include <stdlib.h>
#include <gp2xregs.h>
#include <orcus.h>

int main() {
  orcus_init();

  void* a = malloc(100);
  void* b = malloc(100);
  orcus_uart_printf("first malloc at: 0x%08x\r\n", a);
  orcus_uart_printf("second malloc at: 0x%08x\r\n", b);
  
  orcus_uart_printf("Adan is %d\r\n", 103);
  orcus_uart_printf("Bob is %d\r\n", 583);
  
  while(1);
}
