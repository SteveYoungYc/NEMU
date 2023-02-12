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

def_EHelper(blt) {
    if (*dsrc1 < *dsrc2) {
        rtl_j(s, s->pc + id_dest->imm);
    }
}

def_EHelper(bge) {
    if (*dsrc1 >= *dsrc2) {
        rtl_j(s, s->pc + id_dest->imm);
    }
}

def_EHelper(bltu) {
    if ((word_t)*dsrc1 < (word_t)*dsrc2) {
        rtl_j(s, s->pc + id_dest->imm);
    }
}

def_EHelper(bgeu) {
    if ((word_t)*dsrc1 >= (word_t)*dsrc2) {
        rtl_j(s, s->pc + id_dest->imm);
    }
}