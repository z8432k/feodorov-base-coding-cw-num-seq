#define _GNU_SOURCE

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include "include/array.h"

static void posCounter(double element, size_t idx, void *counter);
static void findLastZero(double element, size_t idx, void *zeroIdx);
static void sum(double element, size_t idx, void *sum);
static int comparator(const void * a, const void *b);
static void printArr(NumArr_t *arr);
static void printEl(double el, size_t idx, void *data);
static void bubbleSort(double array[], size_t asize, int (*comparator)(const void *a, const void *b));

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

  /*  sortArr(arr, comparator); */
  bubbleSort(arr->data, arr->size, comparator);

  printf("\n\n");
  printf("Positive numbers: %zd\n", posCount);

  if (lastZeroIdx >= 0) {
    printf("After last zero summ: %.3lf\n", afterZeroSum);
  }
  else if (lastZeroIdx < 0) {
    printf("After zero sum not calculated.\n");
  }

  printf("Sorted array:\n");
  printArr(arr);

  freeArr(arr);

  return 0;
}


static void posCounter(double element, size_t idx, void *counter) {
  if (element > 0) {
    (*((size_t *) counter))++;
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

static void printArr(NumArr_t *arr) {
  printf("[\n");
  eachArr(arr, 0, NULL, printEl);
  printf("]\n\n");
}

static void printEl(double el, size_t idx, void *data) {
  printf("\t[%3zd, %5.3lf]\n", idx, el);
}

static void bubbleSort(double array[], size_t asize, int (*comparator)(const void *a, const void *b)) {
  double tmp;
  bool swap = false;

  size_t i = 0;
  for (; i < asize - 1; i++) {

    size_t j = 0;
    for (; j < asize - i - 1; j++) {
      int needSwap = comparator(&array[j], &array[j + 1]);

      if (needSwap > 0) {
        tmp = array[j];
        array[j] = array[j + 1];
        array[j + 1] = tmp;
      }
    }
  }
}