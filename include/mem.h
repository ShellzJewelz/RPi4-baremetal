#ifndef MEM_H
#define MEM_H

void mem_init();
void* mem_alloc(unsigned int size);
void mem_free(void* ptr);

#endif // MEM_H