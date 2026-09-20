# Vectors In C

An implementation of vectors (dynamic arrays) in C.
Functions as a single file you can copy and paste.

This is an stb-style library, under the MIT License.
For more information, check the LICENSE.

## Usage

Copy the `vectors.h` file into your project.

When you wish to declare a vector named `v` of type `T`, it is declared as such:

```
T* v = NULL;
```

---

You may initialize your vector with a known size, say `size`.
If you do not do this, `vector_append` gives you a vector with the default size.

```
vector_create_with_capacity(v, size);
```

---

When you wish to add an element, say `e`, use:

```
vector_append(v, e);
```

---

You may see the length of the stored vector using vector_len:

```
vector_len(v); // < this will return 1, since we only have 1 element
```

---

You may access the elements similar to array accesses:

```
v[0]; // < this will be e
```

However, the more robust way to access it is with `vector_get`.
The plus point when doing this an additional safety check.

```
vector_get(v, 0); // < this will be e
vector_get(v, 100); // out of bounds: this will error if it's a debug build
                    // and return 0 in production
```

---

You may pop (remove and return) the last element of the vector with `vector_pop`.

```
vector_pop(v); // < this will be e
vector_len(v); // this will now return 0, since e was already popped
```

---

You may remove the last element with `vector_remove_last`.

```
// assume a vector v1 of length 5: [1, 2, 3, 4, 5]
vector_remove_last(v1);
vector_len(v1); // this will now return 4
// v1 = [1, 2, 3, 4]
```

---

You may remove any element with `vector_remove`.

```
// v1 = [1, 2, 3, 4]
vector_remove(v1, 1);
// v1 = [1, 3, 4]
```

---

You must free the elements you have declared using `vector_free`:

```
vector_free(v);
```

> Important!: function arguments must not have side effects

> Note: `debug` and `prod` refers to whether the macro NDEBUG is unset or set.

> Note: "Side effects" are the unintended results of the code that have been documented.
These may change without further notice when there is a major version bump.


## Example

```
#include <stdio.h>
#include "vectors.h"

int main() {
    int* numbers = NULL;
    int ch;

    do {
        scanf(" %d", &ch);
        vector_append(numbers, ch);

        printf("Continue? (enter 0 to exit) ");
        scanf("%d", &ch);
    } while (ch);

    for (int i = 0; i < vector_len(numbers); i++) {
        printf("Number %d: %d\n", i, vector_get(numbers, i));
    }

    vector_free(numbers);
}
```

## Limitations

`vectors.h` functions by prepending the produced array with a header.
When the array overflows, the vector is realloced.
This reallocation may be at a different memory address.

Therefore, passing a vector into a function where it could be appended
**and not returning the realloced vector**
leads to undefined behaviour.
