#ifndef BUMP_H
#define BUMP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define KiB(size) ((size_t) (size) * 1024)
#define MiB(size) ((KiB(size) * 1024))
#define GiB(size) ((MiB(size) * 1024))
#define HEAP_CAPACITY (KiB(512))

typedef struct Chunk Chunk;
struct Chunk {
    size_t size;
};

static Chunk HEAP[HEAP_CAPACITY];
static size_t HEAP_TOP = 0;

static void *bump_malloc(size_t size) {
    if (HEAP_TOP + size > HEAP_CAPACITY) {
        return NULL;
    }
    Chunk *chunk = &HEAP[HEAP_TOP];
    chunk->size = size;
    HEAP_TOP += size;
    return chunk;
}

static void *bump_calloc(size_t nmemb, size_t size) {
    size_t total = nmemb * size;
    void *ptr = bump_malloc(total);
    if (ptr == NULL) {
        return NULL;
    }
    memset(ptr, 0, total);
    return ptr;
}

static void *bump_realloc(void *ptr, size_t size) {
    if (HEAP_TOP + size > HEAP_CAPACITY) {
        return NULL;
    }
    void *new_loc = bump_malloc(size);
    memcpy(new_loc, ptr, size);
}

static void bump_free(void *ptr) {
    Chunk *chunk = (Chunk *) ptr;
    memset(chunk, 0, chunk->size);
}

static void bump_free_all(void) {
    memset(HEAP, 0, HEAP_CAPACITY);
    HEAP_TOP = 0;
}

#endif // BUMP_H
