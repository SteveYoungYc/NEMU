def_EHelper(csrrw) {
  rtl_csrrw(s, dsrc1, id_src2->imm, ddest);
}

def_EHelper(csrrs) {
  rtl_csrrs(s, dsrc1, id_src2->imm, ddest);
}

def_EHelper(csrrc) {
  rtl_csrrc(s, dsrc1, id_src2->imm, ddest);
}

def_EHelper(csrrwi) {
  rtl_csrrwi(s, id_src1->imm, id_src2->imm, ddest);
}

def_EHelper(csrrsi) {
  rtl_csrrsi(s, id_src1->imm, id_src2->imm, ddest);
}

def_EHelper(csrrci) {
  rtl_csrrci(s, id_src1->imm, id_src2->imm, ddest);
}