def_EHelper(jal) {
    *s0 = s->pc;
    rtl_addi(s, ddest, s0, 4);
    rtl_j(s, s->pc + id_src1->imm);
}

def_EHelper(jalr) {
    *s0 = s->pc;
    rtl_addi(s, ddest, s0, 4);
    rtl_j(s, *dsrc1 + id_src2->imm);
}
