#include <stdlib.h>

#define INIT_DATA_SLOTS 3

typedef struct {
  size_t size;
  size_t allocated;
  double *data;
} NumArr_t;

void createArr(NumArr_t **arr);
void pushArr(NumArr_t *arr, const double number);
void inputArr(NumArr_t **ptr);
void eachArr(const NumArr_t *arr, const size_t offset, void *data, void (*callback)(double, size_t, void *));
void sortArr(const NumArr_t *arr, int (*comparator)(const void *a, const void *b));