#include "utils.h"

void memcpy(void* dst, void* src, unsigned int size)
{
    char* temp_dst = (char*)dst;
    char* temp_src = (char*)src;

    while (size)
    {
        *temp_dst = *temp_src;
        size--;
    }
}