#ifndef __RISCV32_REG_H__
#define __RISCV32_REG_H__

#include <common.h>

static inline int check_reg_idx(int idx) {
  IFDEF(CONFIG_RT_CHECK, assert(idx >= 0 && idx < 32));
  return idx;
}

#define gpr(idx) (cpu.gpr[check_reg_idx(idx)]._32)

#define csr_mtvec   &cpu.mtvec
#define csr_mepc    &cpu.mepc
#define csr_mcause  &cpu.mcause

#define get_sr(idx) (idx == 0x305 ? &cpu.mtvec :    \
                    (idx == 0x341 ? &cpu.mepc :     \
                    (idx == 0x342 ? &cpu.mcause : NULL)))

static inline const char* reg_name(int idx, int width) {
  extern const char* regs[];
  return regs[check_reg_idx(idx)];
}

#endif
