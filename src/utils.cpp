#include "utils.h"

void memcpy(void* dst, void* src, unsigned long int size)
{
    if (size % sizeof(unsigned long int) == 0)
    {
        unsigned long int* temp_dst = (unsigned long int*)dst;
        unsigned long int* temp_src = (unsigned long int*)src;

        while (size)
        {
            *temp_dst = *temp_src;
            size -= sizeof(unsigned long int);
            temp_dst++;
            temp_src++;
        }
    }
    else
    {
        unsigned char* temp_dst = (unsigned char*)dst;
        unsigned char* temp_src = (unsigned char*)src;

        while (size)
        {
            *temp_dst = *temp_src;
            size--;
            temp_dst++;
            temp_src++;
        }
    }
}