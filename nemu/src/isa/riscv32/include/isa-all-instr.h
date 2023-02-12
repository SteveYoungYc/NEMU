#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(inv) f(nemu_trap) \
                    f(addi) f(sltiu) \
                    f(auipc) f(jal) f(jalr) \
                    f(add) f(sub) \
                    f(beq) f(bne)
def_all_EXEC_ID();
