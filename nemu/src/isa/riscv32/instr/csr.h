def_EHelper(csrrw) {
  rtlreg_t *csr = get_sr(id_src2->imm);
  assert(csr != NULL);
  rtl_csrrw(s, dsrc1, csr, ddest);
}

def_EHelper(csrrs) {
  rtlreg_t *csr = get_sr(id_src2->imm);
  assert(csr != NULL);
  rtl_csrrs(s, dsrc1, csr, ddest);
}

def_EHelper(csrrc) {
  rtlreg_t *csr = get_sr(id_src2->imm);
  assert(csr != NULL);
  rtl_csrrc(s, dsrc1, csr, ddest);
}

def_EHelper(csrrwi) {
  rtlreg_t *csr = get_sr(id_src2->imm);
  assert(csr != NULL);
  rtl_csrrwi(s, id_src1->imm, csr, ddest);
}

def_EHelper(csrrsi) {
  rtlreg_t *csr = get_sr(id_src2->imm);
  assert(csr != NULL);
  rtl_csrrsi(s, id_src1->imm, csr, ddest);
}

def_EHelper(csrrci) {
  rtlreg_t *csr = get_sr(id_src2->imm);
  assert(csr != NULL);
  rtl_csrrci(s, id_src1->imm, csr, ddest);
}