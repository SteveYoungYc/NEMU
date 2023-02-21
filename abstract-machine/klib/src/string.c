#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char* s) {
  register const char* c;
  for (c = s; *c; ++c);
  return (c - s);
}

char* strcpy(char* dst, const char* src) {
  char* save = dst;
  for (; (*dst = *src); ++src, ++dst);
  return (save);
}

char* strncpy(char* dst, const char* src, size_t n) {
  if (n != 0) {
    register char* d = dst;
    register const char* s = src;
    do {
      if ((*d++ = *s++) == 0) {
        /* NUL pad the remaining n-1 bytes */
        while (--n != 0)
          *d++ = 0;
        break;
      }
    } while (--n != 0);
  }
  return (dst);
}

char* strcat(char* dst, const char* src) {
  char* save = dst;
  for (; *dst; ++dst);
  while ((*dst++ = *src++));
  return (save);
}

int strcmp(const char* s1, const char* s2) {
  while (*s1 == *s2++)
    if (*s1++ == 0)
      return (0);
  return (*(unsigned char*)s1 - *(unsigned char*)--s2);
}

int strncmp(const char* s1, const char* s2, size_t n) {
  if (n == 0)
    return (0);
  do {
    if (*s1 != *s2++)
      return (*(unsigned char*)s1 - *(unsigned char*)--s2);
    if (*s1++ == 0)
      break;
  } while (--n != 0);
  return (0);
}

void* memset(void* s, int c, size_t n) {
  unsigned char* p = (unsigned char*)s;
  for (size_t i = 0; i < n; i++) {
    p[i] = c;
  }
  return s;
}

void* memmove(void* dst, const void* src, size_t n) {
  uint8_t* from = (uint8_t*)src;
  uint8_t* to = (uint8_t*)dst;

  if (from == to || n == 0)
    return dst;
  if (to > from && to - from < (int)n) {
    /* to overlaps with from */
    /*  <from......>         */
    /*         <to........>  */
    /* copy in reverse, to avoid overwriting from */
    int i;
    for (i = n - 1; i >= 0; i--)
      to[i] = from[i];
    return dst;
  }
  if (from > to && from - to < (int)n) {
    /* to overlaps with from */
    /*        <from......>   */
    /*  <to........>         */
    /* copy forwards, to avoid overwriting from */
    size_t i;
    for (i = 0; i < n; i++)
      to[i] = from[i];
    return dst;
  }
  memcpy(dst, src, n);
  return dst;
}

void* memcpy(void* out, const void* in, size_t n) {
  unsigned char* d = (unsigned char*)out;
  const unsigned char* s = (const unsigned char*)in;
  for (size_t i = 0; i < n; i++) {
    d[i] = s[i];
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n) {
  if (n != 0) {
    register const unsigned char *p1 = s1, *p2 = s2;
    do {
      if (*p1++ != *p2++)
        return (*--p1 - *--p2);
    } while (--n != 0);
  }
  return (0);
}

#endif
