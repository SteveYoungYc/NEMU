#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint16_t value = inw(KBD_ADDR);
  kbd->keydown = ((value & 0x8000) != 0);
  kbd->keycode = (uint8_t)value;
}
