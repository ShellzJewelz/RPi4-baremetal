#include "mem.h"
#include "uart.h"
#include "utils.h"
#include "hw_config.h"

#define MEM_SIZE (1024 * 1024 * 1024)
#define MEM_PAGE_SIZE (4 * 1024)

typedef struct mem_page
{
    bool allocated;
    void* page_base_address;
} mem_page_t;

static mem_page_t* first_page;
static mem_page_t* last_page;

static mem_page_t* mem_page_find();

void mem_init()
{
    extern unsigned char _end; // linker symbol for the end of kernel code
    unsigned int mem_size = GPU_MEMORY_BASE - (unsigned int)(&_end);
    mem_size = mem_align(mem_size, MEM_PAGE_SIZE);
    unsigned int number_of_pages = mem_size / MEM_PAGE_SIZE;

    first_page = (mem_page_t*)mem_align((unsigned int)(&_end), MEM_PAGE_SIZE);
    last_page = (mem_page_t*) ((char*)first_page + (number_of_pages - 2) * MEM_PAGE_SIZE);

    for (char* current_page = (char*)first_page; current_page <= last_page; current_page += MEM_PAGE_SIZE)
    {
        mem_page_t* page = (mem_page_t*)current_page;
        page->allocated = false;
        page->page_base_address = (void*)current_page;
    }
}

void* kalloc()
{
    mem_page_t* page = mem_page_find();
    void* ptr = nullptr;

    if (page != nullptr)
    {
        page->allocated = true;
        ptr = (void*)((char*)page->page_base_address + sizeof(*page));
    }

    return ptr;
}

void kfree(void* ptr)
{
    if (ptr != nullptr)
    {
        void* page_ptr = (void*)((char*)ptr - sizeof(mem_page_t));
        mem_page_t* page = (mem_page_t*)page_ptr;

        if (page->page_base_address != page_ptr)
        {
            uart_send_string("\nMemory currption occurred or bad ptr\n");
        }
        else
        {
            page->allocated = false;
        }
    }
}

static mem_page_t* mem_page_find()
{
    mem_page_t* page;
    bool found = false;

    for (char* current_page = (char*)first_page; current_page <= last_page && !found; current_page += MEM_PAGE_SIZE)
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

