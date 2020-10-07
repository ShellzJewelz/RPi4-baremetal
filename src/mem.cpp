#include "mem.h"
#include "uart.h"
#include "utils.h"
#include "hw_config.h"

struct mem_page;

typedef struct mem_block
{
    struct mem_page* page;
    struct mem_block* prev;
    unsigned int size;
    void* ptr;
    struct mem_block* next;
} mem_block_t, *mem_block_list_t;

typedef struct mem_page
{
    struct mem_page* prev;
    mem_block_list_t block_list;
    unsigned int size_left;
    struct mem_page* next;
} mem_page_t, *mem_page_list_t;

static mem_page_list_t g_page_list;

static mem_page_t* mem_page_alloc(unsigned int size);
static mem_block_t* mem_block_alloc(mem_page_t* page);
static void mem_block_free(mem_block_t* block);

void mem_init()
{
    extern unsigned char _heap; // linker symbol for the end of kernel code
    extern unsigned char _heap_end; // linker symbol for the end of kernel code

    unsigned long int mem_size = (unsigned long int)(&_heap_end - &_heap);
    mem_size = mem_align(mem_size, MEM_PAGE_SIZE);
    unsigned long int number_of_pages = mem_size / MEM_PAGE_SIZE;

    mem_page_t* first_page = (mem_page_t*)(&_heap);
    mem_page_t* last_page = (mem_page_t*)((char*)first_page + (number_of_pages - 1) * MEM_PAGE_SIZE);

    for (unsigned char* current_page = (unsigned char*)first_page, *prev_page = nullptr; 
            current_page <= (unsigned char*)last_page; current_page += MEM_PAGE_SIZE)
    {
        mem_page_t* page = (mem_page_t*)current_page;
        page->prev = (mem_page_t*)prev_page;
        page->next = (mem_page_t*)(current_page + MEM_PAGE_SIZE);
        page->size_left = MEM_PAGE_SIZE - sizeof(*page);
        page->block_list = nullptr;
    }

    last_page->next = nullptr;

    g_page_list = first_page;
}

void* mem_alloc(unsigned int size)
{
    void* retval = nullptr;

    mem_page_t* page;
    if ((page = mem_page_alloc(size)) != nullptr)
    {
        mem_block_t* block;
        if ((block = mem_block_alloc(page)) != nullptr)
        {
            block->size = size;
            retval = block->ptr;
            page->size_left -= size + sizeof(*block);
        }
    }

    return retval;
}

void mem_free(void* ptr)
{
    mem_block_t* block = (mem_block_t*)((unsigned char*)ptr - sizeof(*block));

    if (ptr != block->ptr)
    {
        uart_puts("Memory Currption Occurred\n");
    }
    else
    {
        mem_block_free(block);
    }
}

static mem_page_t* mem_page_alloc(unsigned int size)
{
    mem_page_t* retval = nullptr;

    bool page_found = false;
    for (mem_page_t* page = g_page_list; !page_found && page != nullptr; page = page->next)
    {
        if (size + sizeof(*page) <= page->size_left)
        {
            page_found = true;
            retval = page;
        }
    }

    return retval;
}

static mem_block_t* mem_block_alloc(mem_page_t* page)
{
    mem_block_t* retval = nullptr;

    if (page->block_list == nullptr)
    {
        // Allocate first block
        retval = page->block_list = (mem_block_t*)((unsigned char*)page + sizeof(*page));
        retval->page = page;
        retval->prev = nullptr;
        retval->next = nullptr;
        retval->ptr = (void*)((unsigned char*)page->block_list + sizeof(*retval));
        retval->size = 0;
    }
    else
    {
        bool block_found = false;
        mem_block_t* block;

        // Find a block that can fit between the other blocks
        for (block = page->block_list; block_found && block->next != nullptr; block = block->next)
        {
            bool gap_between_blocks_exists = (mem_block_t*)((unsigned char*)block->ptr + block->size) 
                != block->next;
            if (gap_between_blocks_exists)
            {
                retval = (mem_block_t*)((unsigned char*)block->ptr + block->size);
                retval->page = page;
                block->next->prev = retval;
                retval->next = block->next;
                block->next = retval;
                retval->prev = block;
                retval->ptr = (void*)((unsigned char*)retval + sizeof(*retval));
                retval->size = 0;
                block_found = true;
            }
        }

        // We can't fit a block append to the end
        if (!block_found && block)
        {
            block->next = retval = (mem_block_t*)((unsigned char*)block->ptr + block->size);
            retval->page = page;
            retval->prev = block;
            retval->next = nullptr;
            retval->ptr = (void*)((unsigned char*)retval + sizeof(*retval));
            retval->size = 0;
        }
    }

    return retval;
}

static void mem_block_free(mem_block_t* block)
{
    block->prev->next = block->next;
    block->page->size_left += sizeof(*block) + block->size;
}