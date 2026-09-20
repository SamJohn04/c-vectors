#include "vectors.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__unix__) || defined(__APPLE__)

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#define subprocess_error(errfn, args)                                          \
  do {                                                                         \
    pid_t pid = fork();                                                        \
    if (pid < 0) {                                                             \
      printf("fork could not be created\n");                                   \
      break;                                                                   \
    } else if (pid == 0) {                                                     \
      int devnull = open("/dev/null", O_WRONLY);                               \
      if (devnull != -1) {                                                     \
        dup2(devnull, STDOUT_FILENO);                                          \
        dup2(devnull, STDERR_FILENO);                                          \
        close(devnull);                                                        \
      }                                                                        \
      errfn(args);                                                             \
      exit(0);                                                                 \
    }                                                                          \
    int status;                                                                \
    waitpid(pid, &status, 0);                                                  \
    vector_assert(WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT,          \
                  "function did not fail with assert");                        \
    display_success();                                                         \
  } while (0)

#endif // defined(__unix__) || defined(__APPLE__)

#define vector_assert(condition, fail_message)                                 \
  if (!(condition)) {                                                          \
    fflush(stdout);                                                            \
    fprintf(stderr, "<console color-red>[\uf00d] %s</console>\n",              \
            fail_message);                                                     \
    return 1;                                                                  \
  }

void display_success();
int *make_vector(int64_t size, int64_t capacity);

#define create_with_cap(args) vector_create_with_capacity(args.vct, args.cap)
#define get_from_vec(args) (void)vector_get(args.vct, args.index)

int test_vector_create_with_capacity_debug() {
  int *vector = NULL;
  printf("vector_create_with_capacity at 128\t");
  vector_create_with_capacity(vector, 128);
  vector_assert(vector != NULL, "vector is NULL");
  vector_assert(_vct_get_header(vector)->capacity == 128,
                "vector capacity is not 128");
  vector_free(vector);
  display_success();

#ifdef subprocess_error
  struct {
    int *vct;
    int64_t cap;
  } args;
  args.vct = NULL;

  printf("vector_create_with_capacity at 0\t");
  args.cap = 0;
  subprocess_error(create_with_cap, args);

  printf("vector_create_with_capacity at -1\t");
  args.cap = -1;
  subprocess_error(create_with_cap, args);
#else
  printf("Some tests have not been run due to the operating system");
  return 2;
#endif
  return 0;
}

int test_vector_len_debug() {
  printf("vector_len with length 8\t");
  int *vector = make_vector(8, 16);
  int i = vector_len(vector);
  vector_assert(i == 8, "vector_len is not 8");
  vector_free(vector);
  display_success();

  printf("vector_len with NULL\t");
  int *vector2 = NULL;
  vector_assert(vector_len(vector2) == 0, "vector_len of NULL is not 0");
  display_success();

  printf("vector_len with length 0\t");
  int *vector3 = make_vector(0, 1);
  vector_assert(vector_len(vector3) == 0, "vector_len is not 0");
  vector_free(vector3);
  display_success();

  return 0;
}

int test_vector_append_debug() {
  printf("vector_append to a NULL vector\t");
  int *vector = NULL;
  vector_append(vector, 128);
  vector_assert(vector != NULL && vector[0] == 128,
                "vector is NULL or incorrect");
  display_success();

  printf("vector_append to a vector of length = 1\t");
  vector_append(vector, 256);
  vector_assert(vector[0] == 128 && vector[1] == 256,
                "vector not in the right order");
  vector_free(vector);
  display_success();

  printf("vector_append to a vector that's full\t");
  int *vector2 = make_vector(8, 8);
  vector_append(vector2, 1);
  vector_assert(vector2[8] == 1, "vector not in the right order");
  display_success();

  return 0;
}

int test_vector_get_debug() {
  printf("vector_get index 1 from a vector of length 3\t");
  int *vector = NULL;
  vector_append(vector, 1);
  vector_append(vector, 2);
  vector_append(vector, 3);
  vector_assert(vector_get(vector, 1) == 2, "vector get not right");
  display_success();

#ifdef subprocess_error
  struct {
    int *vct;
    int64_t index;
  } args;
  args.vct = vector;
  printf("vector_get index 5 from length 3\t");
  args.index = 5;
  subprocess_error(get_from_vec, args);

  printf("vector_get index -1 from length 3\t");
  args.index = -1;
  subprocess_error(get_from_vec, args);
  vector_free(args.vct);

  printf("vector_get index 0 from NULL\t");
  args.vct = NULL;
  args.index = 0;
  subprocess_error(get_from_vec, args);
#else
  printf("Some tests have not been run due to the operating system");
  return 2;
#endif

  return 0;
}

int test_vector_pop_debug() {
  printf("vector_pop from a vector of length 3\t");
  int *vector = NULL;
  vector_append(vector, 1);
  vector_append(vector, 2);
  vector_append(vector, 3);
  vector_assert(vector_pop(vector) == 3 && vector_len(vector) == 2,
                "vector pop not working");
  vector_free(vector);
  display_success();

#ifdef subprocess_error
  printf("vector_pop from a NULL vector\t");
  int *vector2 = NULL;
  subprocess_error(vector_pop, vector2);

  printf("vector_pop from an empty vector\t");
  vector_create_with_capacity(vector2, 1);
  subprocess_error(vector_pop, vector2);

  vector_free(vector2);
#else
  printf("Some tests have not been run due to the operating system");
  return 2;
#endif

  return 0;
}

int test_vector_remove_last_debug() {
  printf("vector_remove_last from a vector of length 3\t");
  int *vector = NULL;
  vector_append(vector, 1);
  vector_append(vector, 2);
  vector_append(vector, 3);
  vector_remove_last(vector);
  vector_assert(vector_len(vector) == 2, "vector remove last not working");
  vector_assert(vector_get(vector, 0) == 1 && vector_get(vector, 1) == 2,
                "vector remove last not working");
  vector_free(vector);
  display_success();

  printf("vector_remove_last from a NULL vector\t");
  int *vector2 = NULL;
  vector_remove_last(vector2);
  display_success();

  printf("vector_remove_last from an empty vector\t");
  vector_create_with_capacity(vector2, 1);
  vector_remove_last(vector2);
  display_success();

  vector_free(vector2);
  return 0;
}

int test_vector_remove_debug() {
  printf("vector_remove 2nd element from a vector of length 3\t");
  int *vector = NULL;
  vector_append(vector, 1);
  vector_append(vector, 2);
  vector_append(vector, 3);
  vector_remove(vector, 1);
  vector_assert(vector_len(vector) == 2, "vector remove not working");
  vector_assert(vector[0] == 1 && vector[1] == 3, "vector remove not working");
  display_success();

  printf("vector_remove 5th element from a vector of 2\t");
  vector_remove(vector, 5);
  vector_assert(vector_len(vector) == 2, "vector out of range");
  display_success();

  printf("vector_remove -1th element\t");
  vector_remove(vector, -1);
  vector_assert(vector_len(vector) == 2, "vector out of range");
  vector_free(vector);
  display_success();

  int *vector2 = NULL;
  printf("vector_remove 0th element from NULL\t");
  vector_remove(vector2, 0);
  vector_assert(vector2 == NULL, "no longer NULL");
  display_success();

  return 0;
}

int test_vector_free_debug() {
  printf("vector free on a vector\t");
  int *vector = NULL;
  vector_create_with_capacity(vector, 255);
  vector_free(vector);
  display_success();

  printf("vector free on NULL\t");
  int *vector2 = NULL;
  vector_free(vector2);
  display_success();

  return 0;
}

int test_vector_create_with_capacity_prod() {
  int *vector = NULL;
  printf("vector_create_with_capacity at 128\t");
  vector_create_with_capacity(vector, 128);
  vector_assert(vector != NULL, "vector is NULL");
  vector_assert(_vct_get_header(vector)->capacity == 128,
                "vector capacity is not 128");
  display_success();

  printf("vector_create_with_capacity at 0\t");
  vector_create_with_capacity(vector, 0);
  vector_assert(vector != NULL, "vector is NULL");
  vector_assert(_vct_get_header(vector)->capacity == VECTOR_INITIAL_CAPACITY,
                "vector initial capacity not matching the default");
  display_success();

  printf("vector_create_with_capacity at -1\t");
  vector_create_with_capacity(vector, -1);
  vector_assert(vector != NULL, "vector is NULL");
  vector_assert(_vct_get_header(vector)->capacity == VECTOR_INITIAL_CAPACITY,
                "vector initial capacity not matching the default");
  display_success();

  return 0;
}

int test_vector_len_prod() {
  // nothing is different so just call test_vector_len_debug
  return test_vector_len_debug();
}

int test_vector_append_prod() {
  // nothing is different so just call test_vector_append_debug
  return test_vector_append_debug();
}

int test_vector_get_prod() {
  printf("vector_get index 1 from a vector of length 3\t");
  int *vector = NULL;
  vector_append(vector, 1);
  vector_append(vector, 2);
  vector_append(vector, 3);
  vector_assert(vector_get(vector, 1) == 2, "vector get not right");
  display_success();

  printf("vector_get index 5 from length 3\t");
  vector_assert(vector_get(vector, 5) == 0, "non-zero");
  display_success();

  printf("vector_get index -1 from length 3\t");
  vector_assert(vector_get(vector, -1) == 0, "non-zero");
  vector_free(vector);
  display_success();

  int *vector2 = NULL;
  printf("vector_get index 0 from NULL\t");
  vector_assert(vector_get(vector2, 0) == 0, "non-zero");
  display_success();

  return 0;
}

int test_vector_pop_prod() {
  printf("vector_pop from a vector of length 3\t");
  int *vector = NULL;
  vector_append(vector, 1);
  vector_append(vector, 2);
  vector_append(vector, 3);
  vector_assert(vector_pop(vector) == 3 && vector_len(vector) == 2,
                "vector pop not working");
  vector_free(vector);
  display_success();

  printf("vector_pop from a NULL vector\t");
  int *vector2 = NULL;
  vector_assert(vector_pop(vector2) == 0, "non-zero");
  display_success();

  printf("vector_pop from an empty vector\t");
  vector_create_with_capacity(vector2, 1);
  vector_assert(vector_pop(vector2) == 0, "non-zero");
  display_success();

  vector_free(vector2);

  return 0;
}

int test_vector_remove_last_prod() { return test_vector_remove_last_debug(); }
int test_vector_remove_prod() { return test_vector_remove_debug(); }
int test_vector_free_prod() { return test_vector_free_debug(); }

int main() {
  int err = 0;

#ifdef NDEBUG
  printf("Test 1: vector_create_with_capacity\n");
  err |= test_vector_create_with_capacity_prod();

  printf("\nTest 2: vector_len\n");
  err |= test_vector_len_prod();

  printf("\nTest 3: vector_append\n");
  err |= test_vector_append_prod();

  printf("\nTest 4: vector_get\n");
  err |= test_vector_get_prod();

  printf("\nTest 5: vector_pop\n");
  err |= test_vector_pop_prod();

  printf("\nTest 6: vector_remove_last\n");
  err |= test_vector_remove_last_prod();

  printf("\nTest 7: vector_remove\n");
  err |= test_vector_remove_prod();

  printf("\nTest 8: vector_free\n");
  err |= test_vector_free_prod();
#else
  printf("Test 1: vector_create_with_capacity\n");
  err |= test_vector_create_with_capacity_debug();

  printf("\nTest 2: vector_len\n");
  err |= test_vector_len_debug();

  printf("\nTest 3: vector_append\n");
  err |= test_vector_append_debug();

  printf("\nTest 4: vector_get\n");
  err |= test_vector_get_debug();

  printf("\nTest 5: vector_pop\n");
  err |= test_vector_pop_debug();

  printf("\nTest 6: vector_remove_last\n");
  err |= test_vector_remove_last_debug();

  printf("\nTest 7: vector_remove\n");
  err |= test_vector_remove_debug();

  printf("\nTest 8: vector_free\n");
  err |= test_vector_free_debug();
#endif

  return err;
}

void display_success() { printf("<console color-green>[\uf42e]</console>\n"); }

int *make_vector(int64_t size, int64_t capacity) {
  int *vct = NULL;
  vector_create_with_capacity(vct, capacity);
  for (int64_t i = 0; i < size; i++) {
    vector_append(vct, i);
  }
  return vct;
}
