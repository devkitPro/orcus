#ifndef __ORCUS_H__
#define __ORCUS_H__

#include <stdint.h>
#include <stdbool.h>

// memory layout from linker and init function
extern uint32_t __heap_start;
extern uint32_t __heap_end;

// orcus system functions
extern void orcus_init();

#endif
