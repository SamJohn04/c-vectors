#include <assert.h>
#include <stdio.h>
#include "vectors.h"

int main() {
    printf("test: vector_create_with_capacity          \t");
    int max_size = 2;
    double* n = NULL;
    vector_create_with_capacity(n, max_size);
    assert(n != NULL && "vector after create is still NULL");
    printf("[SUCCESS]\n");

    printf("test: vector_append                        \t");
    vector_append(n, 1.5);
    printf("[SUCCESS]\n");

    printf("test: vector_len                           \t");
    assert(vector_len(n) == 1 && "vector_len is not 1");
    printf("[SUCCESS]\n");

    printf("test: vector_free                          \t");
    vector_free(n);
    printf("[SUCCESS]\n");

    printf("test: capacity is increased if it overflows\t");
    int* n1;
    vector_create_with_capacity(n1, max_size);
    vector_append(n1, 1);
    vector_append(n1, 2);
    vector_append(n1, 3);
    printf("[SUCCESS]\n");

    vector_free(n1);

    printf("test: append with NULL vector              \t");
    int* n2 = NULL;
    vector_append(n2, 1);
    vector_append(n2, 2);
    vector_append(n2, 3);
    printf("[SUCCESS]\n");

    printf("test: access elements like an array        \t");
    int sum = 0;
    for (int i = 0; i < vector_len(n2); i++) {
        sum += n2[i];
    }
    assert(sum == 6 && "sum of elements is not 6");
    printf("[SUCCESS]\n");

    vector_free(n2);
}
