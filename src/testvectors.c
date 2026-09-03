#include "vectors.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

bool test_basics(const char *end_of_line);
bool test_functions(const char *end_of_line);

int *get_vector(int);
bool put_vector(int *vector);
void append_to_vector(int *vector, int);

int main() {
  char *end_of_line = "\n\t";

  printf("\nTests: Basics\n\t");
  test_basics(end_of_line);

  printf("\nTests: Functions\n\t");
  test_functions(end_of_line);
}

bool test_basics(const char *end_of_line) {
  const int max_size = 2;

  printf("test: vector_create_with_capacity                    \t");
  double *n = NULL;
  vector_create_with_capacity(n, max_size);
  assert(n != NULL && "vector after create is still NULL");
  printf("[SUCCESS]%s", end_of_line);

  printf("test: vector_append                                  \t");
  vector_append(n, 1.5);
  printf("[SUCCESS]%s", end_of_line);

  printf("test: vector_len                                     \t");
  assert(vector_len(n) == 1 && "vector_len is not 1");
  printf("[SUCCESS]%s", end_of_line);

  printf("test: vector_free                                    \t");
  vector_free(n);
  printf("[SUCCESS]%s", end_of_line);

  printf("test: capacity is increased if it overflows          \t");
  int *n1;
  vector_create_with_capacity(n1, max_size);
  vector_append(n1, 1);
  vector_append(n1, 2);
  vector_append(n1, 3);
  printf("[SUCCESS]%s", end_of_line);

  vector_free(n1);

  printf("test: append with NULL vector                        \t");
  int *n2 = NULL;
  vector_append(n2, 1);
  vector_append(n2, 2);
  vector_append(n2, 3);
  printf("[SUCCESS]%s", end_of_line);

  printf("test: access elements like an array                  \t");
  int sum = 0;
  for (int i = 0; i < vector_len(n2); i++) {
    sum += n2[i];
  }
  assert(sum == 6 && "sum of elements is not 6");
  printf("[SUCCESS]%s", end_of_line);

  printf("test: remove an element from the end of the array    \t");
  vector_remove_last(n2);
  assert(vector_len(n2) == 2 && "lens do not match");
  assert(n2[0] == 1 && n2[1] == 2 && "values do not match");
  printf("[SUCCESS]%s", end_of_line);

  printf("test: remove an element from the middle of the array \t");
  vector_append(n2, 3);
  vector_append(n2, 4);
  vector_append(n2, 5);

  vector_remove(n2, 2); // remove the 3rd element
  assert(vector_len(n2) == 4 && "lens do not match");
  assert(n2[0] == 1 && n2[1] == 2 && n2[2] == 4 && n2[3] == 5 &&
         "values do not match");
  printf("[SUCCESS]%s", end_of_line);

  vector_free(n2);
}

bool test_functions(const char *end_of_line) {
  printf("test: calling a function which returns a vector   \t");
  int *v = get_vector(4);
  assert(v != NULL && "returned vector is null");
  printf("[SUCCESS]%s", end_of_line);

  printf("test: calling a function which takes a vector     \t");
  assert(put_vector(v) && "vector in a function is null");
  printf("[SUCCESS]%s", end_of_line);
}

int *get_vector(int n) {
  int *vec = NULL;
  vector_create_with_capacity(vec, n);
  return vec;
}

bool put_vector(int *vector) { return vector != NULL; }
