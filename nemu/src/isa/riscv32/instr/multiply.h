def_EHelper(mul) {
    rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mulh) {
    rtl_muls_hi(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mulsu) {
    
}

def_EHelper(mulu) {
    rtl_mulu_hi(s, ddest, dsrc1, dsrc2);
}

def_EHelper(div) {
    rtl_divs_q(s, ddest, dsrc1, dsrc2);
}

def_EHelper(divu) {
    rtl_divu_q(s, ddest, dsrc1, dsrc2);
}

def_EHelper(rem) {
    rtl_divs_r(s, ddest, dsrc1, dsrc2);
}

def_EHelper(remu) {
    rtl_divu_r(s, ddest, dsrc1, dsrc2);
}
