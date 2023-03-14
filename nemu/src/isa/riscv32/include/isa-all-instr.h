#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(inv) f(nemu_trap) \
                    f(addi) f(xori) f(ori) f(andi) f(slli) f(srli) f(srai) f(slti) f(sltiu) \
                    f(auipc) f(jal) f(jalr) \
                    f(add) f(sub) f(xor) f(or) f(and) f(sll) f(srl) f(sra) f(slt) f(sltu) \
                    f(beq) f(bne) f(blt) f(bge) f(bltu) f(bgeu) \
                    f(mul) f(mulh) f(mulsu) f(mulu) f(div) f(divu) f(rem) f(remu) \
                    f(lb) f(lh) f(lw) f(lbu) f(lhu) f(sb) f(sh) f(sw) \
                    f(csrrw) f(csrrs) f(csrrc) f(csrrwi) f(csrrsi) f(csrrci) f(ecall) f(mret)
def_all_EXEC_ID();
