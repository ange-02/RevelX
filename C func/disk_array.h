#include "disk.h"
#include <math.h>
#ifndef __DISK_ARRAY_H
#define __DISK_ARRAY_H

typedef struct disk_array_t {
  int ndisks;
  disk_t** disks;
  disk_t* pd;
  disk_t* rs;
  int block_size;
} disk_array_t;

disk_array_t * disk_array_create (int ndisks);

void disk_array_addr(
  disk_array_t *disk_array,
  long address,
  int* disk_n,
  long* block_on_disk
);

bool disk_array_write(disk_array_t *disk_array, long address, char * buffer);

void recover_one_disk(disk_array_t *disk_array, disk_t *broken_disk);

#endif