#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#include "disk.h"

bool disk_open(disk_t *disk) {
  disk->fd = open(disk->filename, O_RDWR|O_CREAT, 0777);
  if (disk->fd == -1) {
    return false;
  }

  return true;
}

bool disk_close(disk_t *disk) {
  int ret = close(disk->fd);
  if (ret == -1) {
    return false;
  }

  disk->fd = -1;

  return true;
}

/* Read block from disk, write to *buffer. Return true if successful, false otherwise. */
bool disk_read(disk_t *disk, long block, char *buffer) {
  if (!disk->working) {
    return false;
  }

  size_t s = pread(disk->fd, buffer, disk->block_size, block);
  return s > 0;
}

bool disk_write(disk_t* disk, long block, char* buffer){
  if (!disk->working) {
    return false;
  }

  int worked = pwrite(disk->fd, buffer, disk->block_size, block * disk->block_size);

  if (worked == -1){
    return false;
  }

  return true;
}

