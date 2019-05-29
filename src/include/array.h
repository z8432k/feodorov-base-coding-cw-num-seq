#include <stdlib.h>

#define INIT_DATA_SLOTS 3

typedef struct {
  size_t size;
  size_t allocated;
  double *data;
} NumArr_t;

void inputArr(NumArr_t **ptr);
void eachArr(NumArr_t *arr, size_t offset, void *data, void (*callback)(double, size_t, void *));
void sortArr(NumArr_t *arr, int (*comparator)(const void *a, const void *b));