#include <stdlib.h>
#ifndef __RS_H
#define __RS_H

struct tables_t {
  int* gflog;
  int* gfilog;
};

/* add elements of `arr`, which is length `len` */
int gf_add(int * arr, int len);

int gf_drive(int index);
int gf_mul(int a, int b);
int sub_gf8(int a, int b);
int gf_div(int a, int b);

#endif