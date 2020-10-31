#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#define BIT(x) ((1 << x))

#define ARRAY_LENGTH(a) ((sizeof(a) / sizeof(*a)))

unsigned int inline mem_align(unsigned long int value, unsigned int alignment)
{
    return (value + ((alignment - 1) & ~(alignment - 1)));
}

extern "C" inline unsigned long int get_el()
{
    register unsigned long int el;
    asm ("mrs %0, CurrentEL \n"
         "lsr %0, %0, #2 \n" : "=r"(el));
    return el;
}

void memcpy(void* dst, void* src, unsigned long int size);

extern "C" void memzero(void* dst, unsigned int size);
extern "C" void* memset(void* dst, int value, unsigned int size);
extern "C" void write32(void* dst, unsigned int value);
extern "C" unsigned int read32(void* dst);

#endif // UTILS_H