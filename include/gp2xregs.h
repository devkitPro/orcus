#ifndef __GP2X_REGS_H__
#define __GP2X_REGS_H__

#include <stdint.h>

#define r32 volatile uint32_t*
#define r16 volatile uint16_t*
#define r8 volatile uint8_t*

// interrupt vector
extern r32 _undefined_instruction;
extern r32 _software_interrupt;
extern r32 _prefetch_abort;
extern r32 _data_abort;
extern r32 _not_used;
extern r32 _irq;
extern r32 _fiq;

#endif
