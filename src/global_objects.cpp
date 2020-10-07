extern void(*__init_array_start[])();
extern void(*__init_array_end [])();

extern "C" void GlobalInitialize()
{
    unsigned int count = __init_array_end  - __init_array_start;

    for (unsigned int i = 0; i < count; i++)
    {
        __init_array_start[i]();
    }
}

extern "C" int __cxa_atexit(void *object, void(*destructor)(void*), void *dso_handle)
{
    (void)(object);
    (void)(destructor);
    (void)(dso_handle);
    return 0;
}

void* __dso_handle = nullptr;