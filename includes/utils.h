#ifndef UTILS_H
#define UTILS_H

void memcpy(void* dst, void* src, unsigned int size);

unsigned int inline mem_align(unsigned int value, unsigned int alignment)
{
    return (value + ((alignment - 1) & ~(alignment - 1)));
}

extern "C" unsigned long int get_el();
extern "C" void memzero(void* dst, unsigned int size);
extern "C" void write32(void* dst, unsigned int value);
extern "C" unsigned int read32(void* dst);

#endif // UTILS_H