# Vectors In C

An implementation of vectors (dynamic arrays) in C. Functions as a single file you can copy and paste.

## Usage

Copy the `vectors.h` file directory to your project.

When you wish to declare a vector named `v` of type `T`, it is declared as such:

```
T* v = NULL;
```

---

You may declare your vector with a known size, say `size`. If you do not do this `vector_append` gives you a vector with the default size.

```
vector_create_with_capacity(v, size);
```

---

When you wish to add an element, say `e`, use:

```
vector_append(v, e);
```

---

You may see the length of the stored vector using vector_len

```
vector_len(v); // < this will return 1, since we only have 1 element
```

---

You may access the elements similar to array accesses.

```
v[0]; // < this will be e
```

---

You must free the elements you have declared using `vector_free`

```
vector_free(v);
```

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
        printf("Number %d: %d\n", i, numbers[i]);
    }

    vector_free(numbers);
}
```

