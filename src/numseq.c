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

int main(void) {
  printf("Number sequences.\n");

  NumArr_t *arr;
  inputArr(&arr);

  return 0;
}

static void inputArr(NumArr_t **ptr) {
  NumArr_t *arr = (NumArr_t *) malloc(sizeof(NumArr_t));
  double *data = (double *) calloc(INIT_DATA_SLOTS, sizeof(double));
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

      printf("\t Recived: %.3lf\n", number);

      arr->size++;
      arr->data[arr->size - 1] = number;
    }
    else {
      printf("\tRecived %zd numbers.\n", arr->size);
      break;
    }
  } while (true);

  *ptr = arr;
}