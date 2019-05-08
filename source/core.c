#include <stdint.h>
#include <gp2xregs.h>
#include <orcus.h>

r32 IO_BASE32 = (r32) 0xC0000000;
r16 IO_BASE16 = (r16) 0xC0000000;

extern uint32_t __stack_base;
extern uint32_t __int_stack_size;
extern uint32_t __usr_stack_size;
uint32_t __heap_end;

void orcus_init() {
  // establish memory map
  __heap_end = __stack_base - __int_stack_size - __int_stack_size - __usr_stack_size;

  // set up clock sources and memory settings
  // TODO
}
