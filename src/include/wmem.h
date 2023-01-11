#ifndef WMEM_H
#define WMEM_H
#include <stdint.h>

extern unsigned long __heap_base;

unsigned long * bump_pointer = &__heap_base;

void * malloc(unsigned long n) {
  unsigned long * r = bump_pointer;
  bump_pointer += n;
  return (void *)r;
}

void * operator new(unsigned long size) {
    void * p = malloc(size);
    return p;
}

//void free(void* p) {
//  // lol
//}
#endif // WMEM_H
