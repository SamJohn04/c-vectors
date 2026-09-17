#include "vectors.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define vector_assert(condition, fail_message)                                 \
  if (!(condition)) {                                                          \
    fflush(stdout);                                                            \
    fprintf(stderr, "<console color-red>\uf00d %s</console>\n", fail_message); \
    return 1;                                                                  \
  }

int test_basics(const char *end_of_line);
int test_functions(const char *end_of_line);

int *get_vector(int);
bool put_vector(int *vector);
void append_to_vector(int *vector, int);
bool vector_get_test(const char *end_of_line);
bool vector_get_test_prod(const char *end_of_line);
void display_success(const char *end_of_line);

// will convert the program to debug mode
int main() {
  char *end_of_line = "\n\t";
  int err = 0;

  printf("\nTests: Basics\n\t");
  err |= test_basics(end_of_line);

  printf("\nTests: Functions\n\t");
  err |= test_functions(end_of_line); // only 0 | 0 is 0

  return err;
}

int test_basics(const char *end_of_line) {
  const int max_size = 2;

  printf("test: vector_create_with_capacity                    \t");
  double *n = NULL;
  vector_create_with_capacity(n, max_size);
  vector_assert(n != NULL, "vector after create is still NULL");
  display_success(end_of_line);

  printf("test: vector_append                                  \t");
  vector_append(n, 1.5);
  display_success(end_of_line);

  printf("test: vector_len                                     \t");
  vector_assert(vector_len(n) == 1, "vector_len is not 1");
  display_success(end_of_line);

  printf("test: vector_free                                    \t");
  vector_free(n);
  display_success(end_of_line);

  printf("test: capacity is increased if it overflows          \t");
  int *n1;
  vector_create_with_capacity(n1, max_size);
  vector_append(n1, 3);
  vector_append(n1, 2);
  vector_append(n1, 1);
  display_success(end_of_line);

  printf("test: get                                            \t");
  vector_assert(vector_get(n1, 1) == 2, "get failed");
  display_success(end_of_line);

  printf("test: pop                                            \t");
  vector_assert(vector_pop(n1) == 1, "pop result is not 1");
  display_success(end_of_line);

  vector_free(n1);

  printf("test: append with NULL vector                        \t");
  int *n2 = NULL;
  vector_append(n2, 1);
  vector_append(n2, 2);
  vector_append(n2, 3);
  display_success(end_of_line);

  printf("test: access elements like an array                  \t");
  int sum = 0;
  for (int i = 0; i < vector_len(n2); i++) {
    sum += n2[i];
  }
  vector_assert(sum == 6, "sum of elements should be 6");
  display_success(end_of_line);

  printf("test: remove an element from the end of the array    \t");
  vector_remove_last(n2);
  vector_assert(vector_len(n2) == 2, "lens do not match");
  vector_assert(n2[0] == 1 && n2[1] == 2, "values do not match");
  display_success(end_of_line);

  printf("test: remove an element from the middle of the array \t");
  vector_append(n2, 3);
  vector_append(n2, 4);
  vector_append(n2, 5);

  vector_remove(n2, 2); // remove the 3rd element
  vector_assert(vector_len(n2) == 4, "lens do not match");
  vector_assert(n2[0] == 1 && n2[1] == 2 && n2[2] == 4 && n2[3] == 5,
                "values do not match");
  display_success(end_of_line);

  vector_free(n2);

#ifdef NDEBUG
  printf("test: get vector fail: prod build                 \t");
  if (vector_get_test_prod(end_of_line)) {
    return 1;
  }
#else
  printf("test: get vector fail: debug build                \t");
  if (vector_get_test(end_of_line)) {
    return 1;
  }
#endif // NDEBUG

  return 0;
}

int test_functions(const char *end_of_line) {
  printf("test: calling a function which returns a vector   \t");
  int *v = get_vector(4);
  vector_assert(v != NULL, "returned vector is null");
  display_success(end_of_line);

  printf("test: calling a function which takes a vector     \t");
  vector_assert(put_vector(v), "vector in a function is null");
  display_success(end_of_line);

  return 0;
}

int *get_vector(int n) {
  int *vec = NULL;
  vector_create_with_capacity(vec, n);
  return vec;
}

bool put_vector(int *vector) { return vector != NULL; }

bool vector_get_test(const char *end_of_line) {
#if defined(__unix__) || defined(__APPLE__)
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
  int id = fork();
  if (id < 0) {
    printf("<console color-yellow>-</console>%s", end_of_line);
    return 0;
  } else if (id == 0) {
    // is child
    int devnull = open("/dev/null", O_WRONLY);
    if (devnull != -1) {
      dup2(devnull, STDOUT_FILENO);
      dup2(devnull, STDERR_FILENO);
      close(devnull);
    }
    int *v = NULL;
    vector_append(v, 2);
    vector_get(v, 1);

    // test failed
    exit(0);
  }
  int status;
  waitpid(id, &status, 0);
  vector_assert(WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT,
                "status not right");
  display_success(end_of_line);
  return 0;
#endif

  printf("<console color-yellow>-</console>%s", end_of_line);
  return 0;
}

bool vector_get_test_prod(const char *end_of_line) {
  int *v = NULL;
  vector_append(v, 2);
  vector_assert(vector_get(v, 1) == 0, "vector_get prod is wrong");
  return 0;
}

void display_success(const char *end_of_line) {
  printf("<console color-green>\uf42e</console>%s", end_of_line);
}
