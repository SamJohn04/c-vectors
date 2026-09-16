/*
 * VECTORS_H lets the user use dynamic arrays in C.
 * Version 1.1.0
 */

#ifndef VECTORS_H
#define VECTORS_H

#include <assert.h>
#include <stdlib.h>

typedef struct {
  size_t length;
  size_t capacity;
} _vct_header_t;

#define VECTOR_INITIAL_CAPACITY 256

#define _vct_get_header(vector) ((_vct_header_t *)(vector) - 1)

// Create a vector with a known maximum size
// Use this if you do not want VECTOR_INITIAL_CAPACITY picked for you
// If you're fine with not choosing an initial capacity yourself,
// just call vector_append directly
#define vector_create_with_capacity(vector, max_size)                          \
  do {                                                                         \
    _vct_header_t *header =                                                    \
        malloc(sizeof(_vct_header_t) + sizeof(*(vector)) * (max_size));        \
    header->length = 0;                                                        \
    header->capacity = max_size;                                               \
    vector = (void *)(header + 1);                                             \
  } while (0)

// Get the length of a vector
// Must give the start of the vector
#define vector_len(vector) _vct_get_header(vector)->length

// Append an element to the vector
// Must give the start of the vector and the element
#define vector_append(vector, element)                                         \
  do {                                                                         \
    if ((vector) == NULL) {                                                    \
      vector_create_with_capacity(vector, VECTOR_INITIAL_CAPACITY);            \
    }                                                                          \
    _vct_header_t *header = _vct_get_header(vector);                           \
    header->length++;                                                          \
    if (header->length >= header->capacity) {                                  \
      header->capacity *= 2;                                                   \
      header = realloc(header, sizeof(_vct_header_t) +                         \
                                   sizeof(*(vector)) * (header->capacity));    \
      vector = (void *)(header + 1);                                           \
    }                                                                          \
    (vector)[header->length - 1] = (element);                                  \
  } while (0)

// Get the index'th element, if it exists
// If it doesn't and you're in DEBUG build, it errors out
// If it doesn't and you're in PROD build, it returns 0
#define vector_get(vector, index)                                              \
  assert((index) >= 0 && (index) < _vct_get_header(vector)->length),           \
      (index) >= 0 && (index) < _vct_get_header(vector)->length                \
          ? (vector)[(index)]                                                  \
          : 0

// Pop the last element of the vector, returning it
#define vector_pop(vector)                                                     \
  (_vct_get_header(vector)->length--, vector[_vct_get_header(vector)->length])

// Remove the last element of the vector *without* returning it
#define vector_remove_last(vector) _vct_get_header(vector)->length--

// Remove any element of the vector *without* returning it
#define vector_remove(vector, index)                                           \
  do {                                                                         \
    for (int i = (index); i < vector_len(vector) - 1; i++) {                   \
      (vector)[i] = (vector)[i + 1];                                           \
    }                                                                          \
    vector_remove_last(vector);                                                \
  } while (0)

// free the vector
// Must give the start of the vector
#define vector_free(vector) free(_vct_get_header(vector))

#endif // VECTORS_H
