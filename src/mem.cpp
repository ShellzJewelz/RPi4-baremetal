#include "mem.h"
#include "uart.h"
#include "utils.h"
#include "hw_config.h"

typedef struct mem_page
{
    bool allocated;
    void* page_base_address;
} mem_page_t;

static mem_page_t* first_page;
static mem_page_t* last_page;

static mem_page_t* mem_page_alloc();

void mem_init()
{
    extern unsigned char _heap; // linker symbol for the end of kernel code
    extern unsigned char _stack; // linker symbol for the end of kernel code

    unsigned long int mem_size = (unsigned long int)(&_stack - &_heap);
    mem_size = mem_align(mem_size, MEM_PAGE_SIZE);
    unsigned long int number_of_pages = mem_size / MEM_PAGE_SIZE;

    first_page = (mem_page_t*)(&_heap);
    last_page = (mem_page_t*)((char*)first_page + (number_of_pages - 2) * MEM_PAGE_SIZE);

    for (unsigned char* current_page = (unsigned char*)first_page; 
            current_page <= (unsigned char*)last_page; current_page += MEM_PAGE_SIZE)
    {
        mem_page_t* page = (mem_page_t*)current_page;
        page->allocated = false;
        page->page_base_address = (void*)current_page;
    }
}

void* mem_alloc()
{
    mem_page_t* page = mem_page_alloc();
    void* ptr = nullptr;

    if (page != nullptr)
    {
        page->allocated = true;
        ptr = (void*)((char*)page->page_base_address + sizeof(*page));
    }

    return ptr;
}

void mem_free(void* ptr)
{
    if (ptr != nullptr)
    {
        void* page_ptr = (void*)((char*)ptr - sizeof(mem_page_t));
        mem_page_t* page = (mem_page_t*)page_ptr;

        if (page->page_base_address != page_ptr)
        {
            uart_puts("\nMemory currption occurred or bad ptr\n");
        }
        else
        {
            page->allocated = false;
        }
    }
}

static mem_page_t* mem_page_alloc()
{
    mem_page_t* page;
    bool found = false;

    for (unsigned char* current_page = (unsigned char*)first_page; 
            current_page <= (unsigned char*)last_page && !found; current_page += MEM_PAGE_SIZE)
    {
        page = (mem_page_t*)current_page;
        found = !page->allocated;
    }

    if (!found)
    {
        page = nullptr;
    }
    
    return page;
}

