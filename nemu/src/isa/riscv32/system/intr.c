#include <isa.h>
#include <../local-include/reg.h>

word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  *csr_mepc = epc;
  *csr_mcause = NO;
  return *csr_mtvec;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
