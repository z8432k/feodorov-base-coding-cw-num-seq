#include <stdio.h>
#include <stdlib.h>
#include "include/error.h"

void bang(const char* msg) {
  fprintf(stderr, "%s\n", msg);
  exit(EXIT_FAILURE);
}