#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include <sys/types.h>

#include "include/error.h"

#define INIT_DATA_SLOTS 3

typedef struct {
  size_t size;
  size_t allocated;
  double *data;
} NumArr_t;

static void inputArr(NumArr_t **ptr);
static void eachArr(NumArr_t *arr, size_t offset, void *data, void (*callback)(double, size_t, void *));
static void posCounter(double element, size_t idx, void *counter);
static void findLastZero(double element, size_t idx, void *zeroIdx);
static void sum(double element, size_t idx, void *sum);
static int comparator(const void * a, const void *b);
static void printArr(NumArr_t *arr);

int main(void) {
  printf("Number sequences.\n");

  NumArr_t *arr;
  inputArr(&arr);

  size_t posCount = 0;
  double afterZeroSum = 0;
  ssize_t lastZeroIdx = -1;

  eachArr(arr, 0, &posCount, posCounter);
  eachArr(arr, 0, &lastZeroIdx, findLastZero);

  if (lastZeroIdx >= 0) {
    printf("\tLast zero idx: %zd\n", lastZeroIdx);
    eachArr(arr, lastZeroIdx, &afterZeroSum, sum);
  }
  else {
    printf("\tZero element not found.");
  }

  qsort(arr->data, arr->size, sizeof(double), comparator);

  printf("\n\n");
  printf("Positive numbers: %zd\n", posCount);
  printf("After last zero summ: %.3lf\n", afterZeroSum);
  printf("Sorted array:\n");
  printArr(arr);

  return 0;
}

static void inputArr(NumArr_t **ptr) {
  NumArr_t *arr = (NumArr_t *) malloc(sizeof(NumArr_t));
  double *data = (double *) calloc(INIT_DATA_SLOTS, sizeof(double));

  if (arr == NULL || data == NULL) {
    bang(strerror(errno));
  }

  arr->allocated = INIT_DATA_SLOTS;
  arr->size = 0;
  arr->data = data;

  char *line = NULL;
  size_t bufLen = 0;
  ssize_t lineSize;
  double number;

  printf("Input numbers one by one. Press enter after each. Press enter for finish: \n");

  do {
    lineSize = getline(&line, &bufLen, stdin);

    number = strtod(line, NULL);

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
    else {
      printf("\tRecived %zd numbers.\n", arr->size);
      break;
    }
  } while (true);

  *ptr = arr;
}

static void eachArr(NumArr_t *arr, size_t offset, void *data, void (*callback)(double, size_t, void *)) {
  size_t i = 0 + offset;
  double element;
  for (; i < arr->size; i++) {
    element = arr->data[i];
    (*callback)(element, i, data);
  }
}

static void posCounter(double element, size_t idx, void *counter) {
  if (element > 0) {
    *((size_t *) counter) += 1;
  }
}

static void findLastZero(double element, size_t idx, void *lastZeroIdx) {
  if (element == 0) {
    *((ssize_t *) lastZeroIdx) = idx;
  }
}

static void sum(double element, size_t idx, void *sum) {
  *((double *) sum) += element;
}

static int comparator(const void *a, const void *b) {
  int result = 0;
  double aInt, bInt;

  modf(*(double *) a, &aInt);
  modf(*(double *) b, &bInt);

  result -= aInt <= 1;
  result += bInt <= 1;

  return result;
}

static void printEl(double el, size_t idx, void *data) {
  printf("\t[%3zd, %5.3lf]\n", idx, el);
}

static void printArr(NumArr_t *arr) {
  printf("[\n");
  eachArr(arr, 0, NULL, printEl);
  printf("]\n\n");
}