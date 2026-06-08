/*
 * VECTORS_H lets the user use dynamic arrays in C.
 */

#ifndef VECTORS_H
#define VECTORS_H

#include <stdlib.h>

typedef struct {
    size_t length;
    size_t capacity;
} _vct_header_t;

#define VECTOR_INITIAL_CAPACITY 256

// Create a vector with a known maximum size
// Use this if you do not want VECTOR_INITIAL_CAPACITY picked for you
// If you're fine with not choosing an initial capacity yourself,
// just call vector_append directly
#define vector_create_with_capacity(vector, max_size)                                       \
    do {                                                                                    \
    _vct_header_t* header = malloc(sizeof(_vct_header_t) + sizeof(*(vector)) * (max_size)); \
    header->length = 0;                                                                     \
    header->capacity = max_size;                                                            \
    vector = (void*)(header+1);                                                             \
    } while(0)

// Get the length of a vector
// Must give the start of the vector
#define vector_len(vector) ((_vct_header_t*)(vector) - 1)->length

// Append an element to the vector
// Must give the start of the vector and the element
#define vector_append(vector, element)                                                                           \
    do {                                                                                                         \
        if ((vector) == NULL) {                                                                                  \
            vector_create_with_capacity(vector, VECTOR_INITIAL_CAPACITY);                                        \
        }                                                                                                        \
        _vct_header_t* header = ((_vct_header_t*)(vector) - 1);                                                  \
        header->length++;                                                                                        \
        if (header->length >= header->capacity) {                                                                \
            header->capacity *= 2;                                                                               \
            header            = realloc(header, sizeof(_vct_header_t) + sizeof(*(vector)) * (header->capacity)); \
            vector            = (void*)(header+1);                                                               \
        }                                                                                                        \
        (vector)[header->length - 1] = (element);                                                                \
    } while (0)

// free the vector
// Must give the start of the vector
#define vector_free(vector) free((_vct_header_t*)(vector) - 1)

#endif // VECTORS_H
