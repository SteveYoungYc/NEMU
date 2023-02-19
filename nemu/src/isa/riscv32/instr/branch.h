def_EHelper(beq) {
    if (*dsrc1 == *dsrc2) {
        rtl_j(s, s->pc + id_dest->imm);
    }
}

def_EHelper(bne) {
    if ((sword_t)*dsrc1 != (sword_t)*dsrc2) {
        rtl_j(s, s->pc + id_dest->imm);
    }
}

def_EHelper(blt) {
    if ((sword_t)*dsrc1 < (sword_t)*dsrc2) {
        rtl_j(s, s->pc + id_dest->imm);
    }
}

def_EHelper(bge) {
    if ((sword_t)*dsrc1 >= (sword_t)*dsrc2) {
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