def_EHelper(jal) {
  rtl_addi(s, id_dest->preg, &(s->pc), 4);
  // important: need to fresh the pc register
  rtl_addi(s, s0, &(s->pc), c_sext(id_src1->imm, 11));
  rtl_j(s, *s0);
}

def_EHelper(jalr) {
  rtl_addi(s, s0, &(s->pc), 4);

  rtl_addi(s, s1, id_src1->preg, c_sext(id_src2->imm, 20));
  rtl_andi(s, s1, s1, -2); //(sword_t)-2 is (word_t)0xfffffffe
  rtl_j(s, *s1);
  rtl_mv(s, id_dest->preg, s0);
}
