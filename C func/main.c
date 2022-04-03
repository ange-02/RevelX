#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "disk.h"
#include "disk_array.h"

int main(void) {
  printf("Starting...\n");
  disk_array_t * disk_array = disk_array_create(5);

  disk_array_write(disk_array, 0, "a");
  disk_array_write(disk_array, 1, "b");
  disk_array_write(disk_array, 2, "c");
  disk_array_write(disk_array, 3, "d");
  disk_array_write(disk_array, 4, "e");
  disk_array_write(disk_array, 5, "f");

  printf("setting disk[0] as not working.\n");
  disk_array->disks[0]->working = false;

  printf("recovering disk 0\n");
  recover_one_disk(disk_array, disk_array->disks[0]);
  printf("recovered!\n");
}
