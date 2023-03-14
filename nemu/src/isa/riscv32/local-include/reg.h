#ifndef __RISCV32_REG_H__
#define __RISCV32_REG_H__

#include <common.h>

static inline int check_reg_idx(int idx) {
  IFDEF(CONFIG_RT_CHECK, assert(idx >= 0 && idx < 32));
  return idx;
}

#define gpr(idx) (cpu.gpr[check_reg_idx(idx)]._32)

static inline rtlreg_t* get_sr(word_t idx) {    
  switch (idx) {
  case 0x105:
    return &cpu.stvec;
  case 0x141:
    return &cpu.sepc;
  case 0x142:
    return &cpu.scause;
  default:
    return NULL;
  }
}

static inline const char* reg_name(int idx, int width) {
  extern const char* regs[];
  return regs[check_reg_idx(idx)];
}

#endif
