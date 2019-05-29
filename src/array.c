#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#include <sys/types.h>

#include "include/error.h"
#include "include/array.h"

static ssize_t readNumber(double *element) {
  char *line = NULL;
  ssize_t lineSize;
  size_t bufLen = 0;

  lineSize = getline(&line, &bufLen, stdin);
  *element = strtod(line, NULL);

  return lineSize;
}

static void pushArr(NumArr_t *arr, double number) {
  if (arr->size >= arr->allocated) {
    arr->data = (double *) realloc(arr->data, (arr->allocated *= 2) * sizeof(double));
    if (arr->data == NULL) {
      bang(strerror(errno));
    }
  }

  size_t idx = arr->size;

  printf("\t [%3zd] Recived: %.3lf\n", idx, number);

  arr->data[idx] = number;
  arr->size++;
}

void createArr(NumArr_t **arr) {
  *arr = (NumArr_t *) malloc(sizeof(NumArr_t));
  double *data = (double *) calloc(INIT_DATA_SLOTS, sizeof(double));

  if (arr == NULL || data == NULL) {
    bang(strerror(errno));
  }

  (*arr)->allocated = INIT_DATA_SLOTS;
  (*arr)->size = 0;
  (*arr)->data = data;
}

void inputArr(NumArr_t **arr) {
  createArr(arr);

  double number;

  printf("Input numbers one by one. Press enter after each. Press enter for finish: \n");

  do {
    ssize_t lineSize = readNumber(&number);

    if (errno > 0) {
      fprintf(stderr, "\tError: %s\nTry again:\n", strerror(errno));
      errno = 0;
      continue;
    }

    if (!isfinite(number)) {
      fprintf(stderr, "\tBad number %lf. Try again:\n", number);
      continue;
    }

    if (lineSize > 1) {
      pushArr(*arr, number);
    }
    else {
      printf("\tRecived %zd numbers.\n", (*arr)->size);
      break;
    }
  } while (true);
}

void eachArr(NumArr_t *arr, size_t offset, void *data, void (*callback)(double, size_t, void *)) {
  size_t i = 0 + offset;
  double element;
  for (; i < arr->size; i++) {
    element = arr->data[i];
    (*callback)(element, i, data);
  }
}

void sortArr(NumArr_t *arr, int (*comparator)(const void *a, const void *b)) {
  qsort(arr->data, arr->size, sizeof(double), comparator);
}