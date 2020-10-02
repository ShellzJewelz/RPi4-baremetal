#ifndef UTILS_H
#define UTILS_H

extern "C" void memzero(unsigned long int dst, unsigned long int size);
extern "C" void write32(unsigned long int dst, unsigned int value);
extern "C" unsigned int read32(unsigned long int dst);

#endif // UTILS_H