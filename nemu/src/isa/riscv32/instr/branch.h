def_EHelper(beq) {
    if (*dsrc1 == *dsrc2) {
        rtl_j(s, s->pc + id_dest->imm);
    }
}

def_EHelper(bne) {
    if (*dsrc1 != *dsrc2) {
        rtl_j(s, s->pc + id_dest->imm);
    }
}