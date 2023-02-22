#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char* fmt, ...) {
  panic("Not implemented");
}

#define _ADDRESSOF(v) (&(v))

#define _INTSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

#define _crt_va_start(ap, v) (ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v))
#define _crt_va_arg(ap, t) (*(t*)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define _crt_va_end(ap) (ap = (va_list)0)

#define _va_start _crt_va_start /* windows stdarg.h */
#define _va_arg _crt_va_arg
#define _va_end _crt_va_end

int _div(long* n, unsigned base) {
  int __res;
  __res = ((unsigned long)*n) % (unsigned)base;
  *n = ((unsigned long)*n) / (unsigned)base;
  return __res;
}

#define do_div(n, base) _div(&n, base)

static inline int isdigit(int ch) {
  return (ch >= '0') && (ch <= '9');
}

static char* number(char* str, long num, int base) {
  char tmp[66];
  const char* digits = "0123456789abcdefghijklmnopqrstuvwxyz";
  int i = 0;
  if (num == 0) {
    tmp[i++] = '0';
  } else {
    while (num != 0) {
      tmp[i++] = digits[do_div(num, base)];
    }
  }
  while (i-- > 0)
    *str++ = tmp[i];
  return str;
}

int vsprintf(char* out, const char* fmt, va_list args) {
  int len;
  unsigned long num;
  int i, base;
  char* str;
  const char* s;

  for (str = out; *fmt; ++fmt) {
    if (*fmt != '%') {
      *str++ = *fmt;
      continue;
    }
    ++fmt;
    /* default base */
    base = 10;

    switch (*fmt) {
      case 's':
        s = _va_arg(args, char*);
        len = strlen(s);
        for (i = 0; i < len; ++i)
          *str++ = *s++;
        continue;

      case '%':
        *str++ = '%';
        continue;

        /* integer number formats - set up the flags and "break" */
      case 'o':
        base = 8;
        break;

      case 'x':
        base = 16;
        break;

      case 'd':
      case 'i':
      case 'u':
        break;

      default:
        *str++ = '%';
        if (*fmt)
          *str++ = *fmt;
        else
          --fmt;
        continue;
    }
    num = _va_arg(args, int);
    str = number(str, num, base);
  }
  *str = '\0';
  return str - out;
}

int sprintf(char* out, const char* fmt, ...) {
  va_list arg;
  int done;
  va_start(arg, fmt);
  done = vsprintf(out, fmt, arg);
  va_end(arg);
  return done;
}

int snprintf(char* out, size_t n, const char* fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char* out, size_t n, const char* fmt, va_list ap) {
  panic("Not implemented");
}

#endif
