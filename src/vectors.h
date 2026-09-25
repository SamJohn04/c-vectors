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
 * equal to VECTOR_INITIAL_CAPACITY. A max_size <= 0 will error out in debug and
 * return a vector of default size in prod
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
 * returning it. Does nothing if the vector is empty.
 * Side effect: returns the old length; or 0 if there was nothing to remove
 *
 * vector_remove(vector, index) - Remove any element of the vector *without*
 * returning it. If index < 0 or index >= length, then this functions as a noop
 *
 * vector_free(vector) - free the vector. This should be called instead of
 * free(vector).
 */

#ifndef VECTORS_H
#define VECTORS_H

#include <assert.h>
#include <stdint.h>

typedef struct {
  int64_t length;
  int64_t capacity;
} _vct_header_t;

#ifndef VECTOR_INITIAL_CAPACITY
#define VECTOR_INITIAL_CAPACITY 256
#endif

#if !(defined(VECTOR_MALLOC) && defined(VECTOR_REALLOC) &&                     \
      defined(VECTOR_FREE)) &&                                                 \
    (defined(VECTOR_MALLOC) || defined(VECTOR_REALLOC) ||                      \
     defined(VECTOR_FREE))
#error                                                                         \
    "defining some (but not all) of VECTOR_MALLOC, VECTOR_REALLOC, VECTOR_FREE; if you take control of one, you need to take control of them all"
#elif !(defined(VECTOR_MALLOC) || defined(VECTOR_REALLOC) ||                   \
        defined(VECTOR_FREE))
#include <stdlib.h>

#define VECTOR_MALLOC(_vct_size) malloc(_vct_size)
#define VECTOR_REALLOC(_vct_header, _vct_size) realloc(_vct_header, _vct_size)
#define VECTOR_FREE(_vct_header) free(_vct_header)
#endif

#define _vct_get_header(vector) ((_vct_header_t *)(vector) - 1)

// Create a vector with a known maximum size.
// Use this if you do not want VECTOR_INITIAL_CAPACITY picked for you.
// If you're fine with not choosing an initial capacity yourself,
// just call vector_append directly.
// A max_size <= 0 errors out in debug mode and sets to the default capacity
// on prod
#define vector_create_with_capacity(vector, max_size)                          \
  do {                                                                         \
    int64_t _vct_capacity = (int64_t)(max_size);                               \
    assert(_vct_capacity > 0);                                                 \
    if (_vct_capacity <= 0) {                                                  \
      _vct_capacity = VECTOR_INITIAL_CAPACITY;                                 \
    }                                                                          \
    _vct_header_t *_vct_header = VECTOR_MALLOC(                                \
        sizeof(_vct_header_t) + sizeof(*(vector)) * (_vct_capacity));          \
    _vct_header->length = 0;                                                   \
    _vct_header->capacity = _vct_capacity;                                     \
    vector = (void *)(_vct_header + 1);                                        \
  } while (0)

// Get the length of a vector. Passing in NULL gets you 0
#define vector_len(vector)                                                     \
  ((vector) == NULL ? 0 : _vct_get_header(vector)->length)

// Append an element to the vector
// If vector is NULL, it creates tthe vector with VECTOR_INITIAL_CAPACITY.
// If the vector is full, it doubles the capacity before adding the vector.
#define vector_append(vector, element)                                         \
  do {                                                                         \
    if ((vector) == NULL) {                                                    \
      vector_create_with_capacity(vector, VECTOR_INITIAL_CAPACITY);            \
    }                                                                          \
    _vct_header_t *_vct_header = _vct_get_header(vector);                      \
    _vct_header->length++;                                                     \
    if (_vct_header->length >= _vct_header->capacity) {                        \
      _vct_header->capacity *= 2;                                              \
      _vct_header = VECTOR_REALLOC(                                            \
          _vct_header, sizeof(_vct_header_t) +                                 \
                           sizeof(*(vector)) * (_vct_header->capacity));       \
      vector = (void *)(_vct_header + 1);                                      \
    }                                                                          \
    (vector)[_vct_header->length - 1] = (element);                             \
  } while (0)

// Get the index'th element, if it exists
// If it doesn't and you're in a debug build, it errors out
// If it doesn't and you're in a prod build, it returns 0
#define vector_get(vector, index)                                              \
  (assert((index) >= 0 && (index) < vector_len(vector)),                       \
   (index) >= 0 && (index) < vector_len(vector) ? (vector)[(index)] : 0)

// Pop the last element of the vector, returning it
// If the vector is empty and you're in a debug build, it errors out
// If the vector is empty and you're in a prod build, it returns 0
#define vector_pop(vector)                                                     \
  (assert(vector_len(vector) > 0),                                             \
   vector_len(vector) > 0 ? vector[--_vct_get_header(vector)->length] : 0)

// Remove the last element of the vector *without* returning it
#define vector_remove_last(vector)                                             \
  vector_len(vector) > 0 ? _vct_get_header(vector)->length-- : 0

// Remove any element of the vector *without* returning it
#define vector_remove(vector, index)                                           \
  do {                                                                         \
    int _vct_idx = (index);                                                    \
    if ((vector) == NULL || _vct_idx < 0 || _vct_idx >= vector_len(vector)) {  \
      break;                                                                   \
    }                                                                          \
    for (int i = _vct_idx; i < vector_len(vector) - 1; i++) {                  \
      (vector)[i] = (vector)[i + 1];                                           \
    }                                                                          \
    vector_remove_last(vector);                                                \
  } while (0)

// free the vector.
// This should be called instead of free(vector)
#define vector_free(vector)                                                    \
  (vector) != NULL ? VECTOR_FREE(_vct_get_header(vector)) : 0

#endif // VECTORS_H
