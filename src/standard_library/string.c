#include <string.h>
#include <stddef.h>
#include <stdint.h>

const char *strchr(const char *str, int chr)
  {
    if (str == NULL) {
      return NULL;
    }

    while (*str) {
      if (*str == (char)chr) {
        return str;
      }

      ++str;
    }

    if ((char)chr == '\0') {
      return str;
    }

    return NULL;

  }

char *strcpy(char *dst, const char *src)
  {
    char *orig_dst = dst;

    while (*src) {
      *dst = *src;
        ++src;
        ++dst;
    }

    *dst = '\0';
    return orig_dst;

  }

size_t strlen(const char *str)
  {
    size_t len = 0;
    while (*str) {
      ++len;
      ++str;
    }

    return len;
  }

int strcmp(const char *a, const char *b)
{
    while (*a && *a == *b) {
        ++a;
        ++b;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

int strncmp(const char *a, const char *b, size_t n)
{
    while (n-- > 0) {
        unsigned char ac = (unsigned char)*a++;
        unsigned char bc = (unsigned char)*b++;

        if (ac != bc) return ac - bc;
        if (ac == '\0') return 0;
    }
    return 0;
}

char *strncpy(char *dst, const char *src, size_t n)
  {
    char *out = dst;
    while (n > 0) {
      if (!*src) {
        break;
      }

      *out = *src;
      ++out;
      ++src;
      --n;
    }

    for(int i = 0; i < (int)n; ++i) {
      *out = '\0';
      ++out;
    }

    return out;
  }
