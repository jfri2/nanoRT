#ifndef USTR_H_
#define USTR_H_

#include <string.h>

// Unsigned string cat function (equivalent to strcat from string.h)
static inline uint8_t* ustrcat(uint8_t *dst, const uint8_t *src)
{
    return (uint8_t*)strcat((char *)dst, (char *)src);
}

// Unsigned string length function (equivalent to strlen from string.h)
static inline uint8_t ustrlen(uint8_t *str)
{
    return (uint8_t)strlen((char *)str);
}

#endif // USTR_H_