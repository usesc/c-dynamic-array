#ifndef CDA_H
#define CDA_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

/*
size = size in elements
*/

#define cdamemcpy(dest, src, n) __builtin_memcpy(dest, src, n)
#define cdamemmov(dest, src, n) __builtin_memmove(dest, src, n)
#define cdainline __attribute__((__always_inline__))

#define _cdahdr(cda) (((cdahdr*)cda)-1)
typedef struct {
        size_t size;
        size_t capacity;
        size_t elemsize;
} cdahdr;

typedef uint8_t u8;

#define cdasize(cda) (_cdahdr(cda)->size)
#define cdainit(type, elems) (type *)cdainit0(sizeof(type), elems)
#define cdafile(filename) cdafile0(filename)
#define cdapush(cda, data, elems)     cdapush0((void**)&cda, data, elems)
#define cdapop(cda, out, elems)       cdapop0 ((void**)&cda, out,  elems)
#define cdains(cda, data, elems, idx) cdains0 ((void**)&cda, data, elems, idx)
#define cdatake(cda, out, elems, idx) cdatake0((void**)&cda, out,  elems, idx)
#define cdafree(cda) {free(_cdahdr(cda)); cda = NULL;}

/* uh-oh, long macro here */
#define cdaset(cda, val, idx) \
        do { (arr)[idx] = (val); \
        if ((idx) > cdasize(cda)) \
                _cdahdr(cda)->size = (idx)+1; \
        } while (0);

void *cdainit0(size_t elem_size, size_t pre_fill);
void *cdafile0(char *filename);
int   cdapush0(void **cda, void *data, size_t elems);
int   cdapop0 (void **cda, void *out,  size_t elems);
int   cdains0 (void **cda, void *data, size_t elems, size_t idx);
int   cdatake0(void **cda, void *out,  size_t elems, size_t idx);

#ifdef CDA_IMPL
#include "cda.c"
#endif

#endif