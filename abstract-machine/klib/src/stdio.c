#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


#define _ADDRESSOF(v) (&(v))

#define _INTSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

#define _crt_va_start(ap, v) (ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v))
#define _crt_va_arg(ap, t) (*(t*)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
#define _crt_va_end(ap) (ap = (va_list)0)

#define _va_start _crt_va_start /* windows stdarg.h */
#define _va_arg _crt_va_arg
#define _va_end _crt_va_end

#define STR_OUT 0
#define UART_OUT 1

int _div(long* n, unsigned base) {
  int __res;
  __res = ((unsigned long)*n) % (unsigned)base;
  *n = ((unsigned long)*n) / (unsigned)base;
  return __res;
}

#define do_div(n, base) _div(&n, base)

static int number(char** str_p, long num, int base) {
  char tmp[66];
  const char* digits = "0123456789abcdefghijklmnopqrstuvwxyz";
  int i = 0;
  char* origin_str = *str_p;
  if (num == 0) {
    tmp[i++] = '0';
  } else {
    while (num != 0) {
      tmp[i++] = digits[do_div(num, base)];
    }
  }
  if (origin_str != NULL) {
    while (i-- > 0) {
      **str_p = tmp[i];
      (*str_p)++;
    }
  } else {
    while (i-- > 0)
      putch(tmp[i]);
  }
  return *str_p - origin_str;
}

int fmt_parser(const char* fmt, va_list args, int out_tpye, char* out) {
  unsigned long num;
  int i, base;
  const char* str_fmt;
  char* str_out;
  const char* s;
  int len = 0;

  str_out = out;
  str_fmt = fmt;
  for (str_fmt = fmt; *str_fmt; ++str_fmt) {
    if (*str_fmt != '%') {
      len++;
      if (out_tpye == STR_OUT) {
        *str_out++ = *str_fmt;
      }
      else if (out_tpye == UART_OUT) {
        putch(*str_fmt);
      }
      continue;
    }
    ++str_fmt;
    /* default base */
    base = 10;

    switch (*str_fmt) {
      case 'c':
        char c = (unsigned char)_va_arg(args, int);
        if (out_tpye == STR_OUT)
          *str_out++ = c;
        else if (out_tpye == UART_OUT)
          putch(c);
        continue;

      case 's':
        s = _va_arg(args, char*);
        int s_len = strlen(s);
        len += s_len;
        for (i = 0; i < s_len; ++i) {
          if (out_tpye == STR_OUT)
            *str_out++ = *s++;
          else if (out_tpye == UART_OUT)
            putch(*s++);
        }
        continue; // for continue

        /* integer number formats - set up the flags and "break" */
      case 'o':
        base = 8;
        break;  // switch break

      case 'x':
        base = 16;
        break;

      case 'd':
      case 'i':
      case 'u':
        break;

      default:
        continue;
    }
    num = _va_arg(args, int);
    len += number(&str_out, num, base);
  }
  if (out_tpye == STR_OUT)
    *str_out = '\0';
  return len;
}

int vsprintf(char* out, const char* fmt, va_list args) {
  return fmt_parser(fmt, args, STR_OUT, out);
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

int printf(const char* fmt, ...) {
  va_list arg;
  int done;
  va_start(arg, fmt);
  done = fmt_parser(fmt, arg, UART_OUT, NULL);
  va_end(arg);
  return done;
}

#endif
