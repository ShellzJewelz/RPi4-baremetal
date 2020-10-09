#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define BIT(x) ((1 << x))

void memcpy(void* dst, void* src, unsigned int size);

unsigned int inline mem_align(unsigned long int value, unsigned int alignment)
{
    return (value + ((alignment - 1) & ~(alignment - 1)));
}

extern "C" unsigned long int get_el();
extern "C" void memzero(void* dst, unsigned int size);
extern "C" void write32(void* dst, unsigned int value);
extern "C" unsigned int read32(void* dst);

#endif // UTILS_H