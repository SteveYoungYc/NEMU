def_EHelper(add) {
  rtl_add(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sub) {
  rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(xor) {
  rtl_xor(s, ddest, dsrc1, dsrc2);
}

def_EHelper(or) {
  rtl_or(s, ddest, dsrc1, dsrc2);
}

def_EHelper(and) {
  rtl_and(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sll) {
  rtl_sll(s, ddest, dsrc1, dsrc2);
}

def_EHelper(srl) {
  rtl_srl(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sra) {
  rtl_sra(s, ddest, dsrc1, dsrc2);
}

def_EHelper(slt) {
  rtl_slt(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sltu) {
  rtl_sltu(s, ddest, dsrc1, dsrc2);
}


def_EHelper(addi) {
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(xori) {
  rtl_xori(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(ori) {
  rtl_ori(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(andi) {
  rtl_andi(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(slli) {
  rtl_slli(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(srli) {
  rtl_srli(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(srai) {
  rtl_srai(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(slti) {
  rtl_slti(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(sltiu) {
  rtl_sltui(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(auipc) {
  rtl_li(s, ddest, id_src1->imm + s->dnpc);
}
