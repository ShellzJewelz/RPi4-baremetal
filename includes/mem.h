#ifndef MEM_H
#define MEM_H

void mem_init();
void* kalloc();
void kfree(void* ptr);

#endif // MEM_H