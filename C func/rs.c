#include <stdlib.h>

struct tables_t {
  int* gflog;
  int* gfilog;
};

struct tables_t tables () {
  int polynomial = 0x11d;
  int s = 8;
  int gf_elements = 1 << s;

  int * gflog = calloc(sizeof(int), gf_elements);
  int * gfilog = calloc(sizeof(int), gf_elements);

  int b = 1;
  for (int i = 0; i < gf_elements; i++) {
    gflog[b] = i & 255;
    gfilog[i] = b & 255;
    b <<= 1;
      
    if (b & gf_elements) {
      b ^= polynomial;
    }
  }

  gflog[1] = 0;
  struct tables_t ret;
  ret.gflog = gflog;
  ret.gfilog = gfilog;
  
  return ret;
}

int gf_add(int * arr, int len) {
  int result = 0;
  for (int i = 0; i < len; i++) {
    result ^= arr[i];
  }
  
  return result;
}

int gf_drive(int index) {
  struct tables_t t = tables();
  return t.gfilog[index];
}

int gf_mul(int a, int b) {
  if (a == 0 || b == 0) {
    return 0;
  }
  
  struct tables_t t = tables();
  
  return t.gfilog[(t.gflog[a] + t.gflog[b]) % 255];
}

int sub_gf8(int a, int b) {
  if (a > b) {
    return a - b;
  } else {
    return (255 - (0 - (a - b)));
  }
}

int gf_div(a, b) {
  struct tables_t t = tables();
  return t.gfilog[sub_gf8(t.gflog[a], t.gflog[b])];
}