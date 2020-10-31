#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

void frame_buffer_init();
void frame_buffer_putc(const char c);
void frame_buffer_printf_putc(void* p, char c);
void frame_buffer_puts(const char* s);

#endif // FRAME_BUFFER_H