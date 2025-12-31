#include <stddef.h>
#include <stddef.h>

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len++]) {}
    return len;
}

void *memchr(const char *s, int c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if ((int)s[i] == c) {
            return (void *)(s + i);
        }
    }

    return NULL;
}