#include <stdlib.h>
#include <stdbool.h>
#ifndef __DISK_H
#define __DISK_H

typedef enum { data_disk, parity_disk, rs_disk } disk_type_t;

typedef struct {
    char* filename; // name of file the "disk" is stored on
    int fd; // file descriptor (set only if disk is open)

    int max_blocks; // maGGximum blocks that can be stored on a file
    size_t block_size; // the size of a block

    disk_type_t type; // data, parity or rs (reed-solomomn)

// Analytics
    int nreads;
    int nwrites;
    bool working; // true: disk works fine, false: disk needs to be recovered

} disk_t;

bool disk_open(disk_t *disk);
bool disk_close(disk_t *disk);
bool disk_read(disk_t *disk, long block, char *buffer);
bool disk_write(disk_t* disk, long block, char* buffer);
#endif
