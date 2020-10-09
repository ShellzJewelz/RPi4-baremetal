#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

extern "C" void exceptions_vector_init();
extern "C" void exceptions_irq_enable();
extern "C" void exceptions_irq_disable();

#endif // EXCEPTIONS_H