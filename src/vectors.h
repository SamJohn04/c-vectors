/*
 * VECTORS_H lets the user use dynamic arrays in C.
 * Version 1.1.0
 *
 * This is an stb-style library, under the MIT License. For more information,
 * check the LICENSE.
 *
 * NOTES
 * - `debug` and `prod` refers to whether the macro NDEBUG is unset or set.
 * - "Side effects" are the unintended results of the code that have been
 *    documented. These may change without further notice when there is a
 *    major version bump.
 *
 * VECTOR_INITIAL_CAPACITY - define it to change the default 256
 *
 * vector_create_with_capacity(vector, max_size) - create a vector `vector` with
 * an initial maximum capacity of `max_size`. This is not necessary if it is
 * equal to VECTOR_INITIAL_CAPACITY
 *
 * vector_len(vector) - get the length (of type size_t) of the vector
 *
 * vector_append(vector, element) - add element to the end of the vector.
 * If the vector is NULL, it creates tthe vector with VECTOR_INITIAL_CAPACITY.
 * If the vector is full, it doubles the capacity before adding the vector.
 *
 * vector_get(vector, element) - Get the index'th element, if it exists.
 * If it doesn't and you're in a debug build, it errors out
 * If it doesn't and you're in a prod build, it returns 0
 *
 * vector_pop(vector) - Pop the last element, returning it.
 * If the vector is empty and you're in a debug build, it errors out
 * If the vector is empty and you're in a prod build, it returns 0
 *
 * vector_remove_last(vector) - Remove the last element of the vector *without*
 * returning it.
 * Side effect: returns the old length; and 0 if there was nothing to remove
 *
 * vector_remove(vector, index) - Remove any element of the vector *without*
 * returning it. If index >= length, then this functions as a noop
 *
 * vector_free(vector) - free the vector. This should be called instead of
 * free(vector).
 */

#ifndef VECTORS_H
#define VECTORS_H

#include <assert.h>
#include <stdlib.h>

typedef struct {
  size_t length;
  size_t capacity;
} _vct_header_t;

#ifndef VECTOR_INITIAL_CAPACITY
#define VECTOR_INITIAL_CAPACITY 256
#endif

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
#define vector_len(vector) _vct_get_header(vector)->length

// Append an element to the vector
// If vector is NULL, it creates tthe vector with VECTOR_INITIAL_CAPACITY.
// If the vector is full, it doubles the capacity before adding the vector.
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
// If it doesn't and you're in a debug build, it errors out
// If it doesn't and you're in a prod build, it returns 0
#define vector_get(vector, index)                                              \
  assert((index) >= 0 && (index) < _vct_get_header(vector)->length),           \
      (index) >= 0 && (index) < _vct_get_header(vector)->length                \
          ? (vector)[(index)]                                                  \
          : 0

// Pop the last element of the vector, returning it
// If the vector is empty and you're in a debug build, it errors out
// If the vector is empty and you're in a prod build, it returns 0
#define vector_pop(vector)                                                     \
  (assert(vector_len(vector) > 0),                                             \
   vector_len(vector) > 0 ? vector[--vector_len(vector)] : 0)

// Remove the last element of the vector *without* returning it
#define vector_remove_last(vector)                                             \
  vector_len(vector) > 0 ? vector_len(vector)-- : 0

// Remove any element of the vector *without* returning it
#define vector_remove(vector, index)                                           \
  do {                                                                         \
    int idx = (index);                                                         \
    for (int i = idx; i < vector_len(vector) - 1; i++) {                       \
      (vector)[i] = (vector)[i + 1];                                           \
    }                                                                          \
    if ((idx) < vector_len(vector)) {                                          \
      vector_remove_last(vector);                                              \
    }                                                                          \
  } while (0)

// free the vector.
// This should be called instead of free(vector)
#define vector_free(vector) free(_vct_get_header(vector))

#endif // VECTORS_H
