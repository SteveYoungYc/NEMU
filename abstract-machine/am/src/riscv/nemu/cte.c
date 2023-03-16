#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  // printf("a7:%x, %x, %x, %x\n", c->GPR1, c->mcause, c->mstatus, c->mepc);
  if (user_handler) {
    Event ev;
    // printf("[c->mcause]%d\n", c->mcause);
    switch (c->mcause) {
    case -1:
      ev.event = EVENT_YIELD;
      break;
    case 0 ... 19:
      ev.event = EVENT_SYSCALL;
      break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
