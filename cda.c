#include "cda.h"

void *cdainit0(size_t elem_size, size_t pre_fill) {
        size_t bytes = sizeof(cdahdr) + (pre_fill * elem_size);

        void *ptr = malloc(bytes);
        if (!ptr) return NULL;

        cdahdr *hdr = (cdahdr*)ptr;
        hdr->size = 0;
        hdr->capacity = pre_fill;
        hdr->elemsize = elem_size;

        return hdr+1;
}

void *cdafile0(char *filename) {
        int fd = open(filename, O_RDWR);

        struct stat st;
        if (fstat(fd, &st) == -1)
        return NULL;

        size_t bytes = sizeof(cdahdr) + st.st_size;
        void *mem = malloc(bytes);
        if (!mem) {
                close(fd);
                return NULL;
        }

        cdahdr *hdr = (cdahdr*)mem;
        ssize_t reader = read(fd, hdr+1, st.st_size);
        if (reader < 0) {
                close(fd);
                free(mem);
                return NULL;
        }

        close(fd);

        hdr->size = st.st_size;
        hdr->capacity = st.st_size;
        hdr->elemsize = 1;

        return hdr+1;
}

void *cdadelta(void *cda, ssize_t delta) {
        if (!cda || delta == 0) return cda;

        cdahdr *hdr = _cdahdr(cda);

        ssize_t newcap = hdr->capacity + delta;
        if (newcap < (ssize_t)hdr->size)
        newcap = hdr->size;

        void *newptr = realloc(hdr, sizeof(cdahdr) + newcap * hdr->elemsize);
        if (!newptr) return NULL;

        hdr = (cdahdr*)newptr;
        hdr->capacity = newcap;

        return hdr + 1;
}

int cdapush0(void **cda, void *data, size_t elems) {
        if (!cda || !(*cda) || !data) return 1; 
        if (elems == 0) return 0;

        if (!(*cda = cdadelta(*cda, elems)))
        return 1;

        cdahdr *hdr = _cdahdr(*cda);

        cdamemcpy((u8*)*cda + (hdr->size * hdr->elemsize),
                data, 
                elems * hdr->elemsize
        );

        hdr->size += elems;
        return 0;
}

int cdapop0(void **cda, void *out, size_t elems) {
        if (!cda || !(*cda)) return 1;
        if (elems == 0) return 0;

        if (!(*cda = cdadelta(*cda, (ssize_t)(-elems))))
        return 1;

        cdahdr *hdr = _cdahdr(*cda);
        
        if (out)
        cdamemcpy(out, 
                (u8*)*cda + (hdr->size - elems) * hdr->elemsize,
                elems * hdr->elemsize
        );

        hdr->size -= elems;
        return 0;
}

int cdains0(void **cda, void *data, size_t elems, size_t idx) {
        if (!cda || !(*cda) || !data) return 1;
        if (elems == 0) return 0;

        if (!(*cda = cdadelta(*cda, elems)))
        return 1;

        cdahdr *hdr = _cdahdr(*cda); 
        if (idx > hdr->size) return 1;

        cdamemmov((u8*)*cda + (idx + elems) * hdr->elemsize,
                (u8*)*cda + idx * hdr->elemsize, 
                (hdr->size - idx) * hdr->elemsize
        );

        cdamemcpy((u8*)*cda + idx * hdr->elemsize,
                data, 
                elems * hdr->elemsize
        );

        hdr->size += elems;
        return 0;
}

int cdatake0(void **cda, void *out, size_t elems, size_t idx) {
        if (!cda || !(*cda)) return 1;
        if (elems == 0) return 0;

        if (!(*cda = cdadelta(*cda, (ssize_t)(-elems))))
        return 1;

        cdahdr *hdr = _cdahdr(*cda); 
        if (idx > hdr->size) return 1;

        if (out) 
        cdamemcpy(out, 
                (u8*)*cda + idx * hdr->elemsize,
                elems * hdr->elemsize
        );

        cdamemmov((u8*)*cda + idx * hdr->elemsize,
                (u8*)*cda + (idx + elems) * hdr->elemsize,
                (hdr->size - idx - elems) * hdr->elemsize
        );

        hdr->size -= elems;
        return 0;
}
