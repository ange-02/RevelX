#include "disk.h"
#include "disk_array.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "rs.h"

void update_rs_disk(disk_array_t *disk_array, long block){
  printf("[update_rs_disk] updating block %ld\n", block);
  char ** blocks = malloc(sizeof(char*) * disk_array->ndisks);
  char * image = malloc(sizeof(char) * disk_array->block_size);

  // for each disk
  for (int i = 0; i < disk_array->ndisks; i++) {
    // spwan a new process
    pid_t id = fork();

    // break if parent process
    if (id != 0)
      continue;
    
    // read the disk block into memory
    blocks[i] = malloc(sizeof(char) * disk_array->block_size);
    disk_read(disk_array->disks[i], block, blocks[i]);

    if (id == 0)
      break;
  }

  printf("[update_rs_disk] calculating image...\n");

  // for each byte
  for (int i = 0; i < disk_array->block_size; i++) {
    // spwan a new process
    pid_t id = fork();

    // skip if parent
    if (id != 0) {
      continue;
    }

    int result = 0;

    // for each disk
    for (int j = 0; j < disk_array->ndisks; j++) {
      // gf_add the multiplications
      result ^= gf_mul(gf_drive(j), blocks[j][i]);
    }

    image[i] = result;

    if (id == 0) {
      break;
    }
  }

  printf("[update_rs_disk] image calculated as: %s\n", image);

  disk_open(disk_array->rs);
  disk_write(disk_array->rs, block, image);
  disk_close(disk_array->rs);
}

void setDiskNotWorking(disk_t *myDisk){

  myDisk->working = false;
  
}

disk_array_t * disk_array_create (int ndisks) {
  disk_array_t * disk_array = malloc(sizeof(disk_array_t));

  disk_array->ndisks = ndisks;
  disk_array->disks = malloc(sizeof(disk_t*) * ndisks); 
  disk_array->pd = disk_array->disks[ndisks - 2];
  disk_array->rs = disk_array->disks[ndisks - 1];
  disk_array->block_size = 32;

  if (disk_array->disks == NULL) {
    return NULL;
  }

  for (int i = 0; i < ndisks; i++) {
    disk_array->disks[i] = malloc(sizeof(disk_t));

    disk_array->disks[i]->filename = malloc(sizeof(char) * 10);
    sprintf(disk_array->disks[i]->filename, "disk-%02d", i);
    
    disk_array->disks[i]->fd = -1;
    disk_array->disks[i]->max_blocks = 1028;
    disk_array->disks[i]->block_size = 32;
    disk_array->disks[i]->nreads = 0;
    disk_array->disks[i]->nwrites = 0;
    disk_array->disks[i]->type = data_disk;
    disk_array->disks[i]->working = true;
  }

  disk_array->pd = malloc(sizeof(disk_t));
  if (disk_array->pd == NULL) return NULL;

  disk_array->rs = malloc(sizeof(disk_t));
  if (disk_array->rs == NULL) return NULL;

  printf("[disk_array_create] created disk array\n");
  printf("[disk_array_create] ndisks: %d, pd: %p, rs: %p\n", ndisks, disk_array->pd, disk_array->rs);

  return disk_array;
}  


void disk_array_addr(disk_array_t *disk_array, long address, int* disk_n, long* block_on_disk) {
  int num_data_disks = disk_array->ndisks - 2;
  *disk_n = address % num_data_disks;
  *block_on_disk = floor(address / num_data_disks);
}

bool disk_array_read(disk_array_t *disk_array, long address, char * buffer) {
  int disk_index; 
  long block;
  disk_array_addr(disk_array, address, &disk_index, &block);
  
  disk_t* disk = disk_array->disks[disk_index];
  disk_open(disk);

  if (disk->working) {
    disk_read(disk, block, buffer);
  } else {
    recover_disk(disk_array, disk);
    disk_read(disk, block, buffer);
  }
  
  return disk_close(disk);
}

bool update_parity_disk(disk_array_t *disk_array, long block_on_disk){
  char* parityData = malloc(sizeof(char) * disk_array->block_size);
  char* buffer = malloc(sizeof(char) * disk_array->block_size);


  bool worked = disk_read(disk_array->disks[0], block_on_disk, buffer);
  
  if (worked == false){
    printf("Didn't work :((\n");
    return false;
  }

  for (int j = 0; j < disk_array->block_size; j++){
    parityData[j] = buffer[j];
  }  
    

  for (int i = 1;  i < disk_array->ndisks - 2; i++){
    disk_read(disk_array->disks[i], block_on_disk, buffer); //Need to do something with true / false if read was unsuccessful

    for(int j = 0; j< disk_array->block_size; j++){
        parityData[j] = parityData[j] ^ buffer[j];
    }
  }

  printf("writing parity data...\n");
  printf("pd filename: %s\n", disk_array->pd->filename);
  disk_array->pd->filename = "disk-03";
  disk_open(disk_array->pd);
  return disk_write(disk_array->pd, block_on_disk, parityData);
}

bool disk_array_write(disk_array_t *disk_array, long address, char * buffer) {
  int disk_index; 
  long block;
  disk_array_addr(disk_array, address, &disk_index, &block);
  
  disk_t* disk = disk_array->disks[disk_index];
  disk_open(disk);
  
  disk_write(disk, block, buffer);
  update_parity_disk(disk_array, block);
  update_rs_disk(disk_array, block);

  return true;
}




/* update one block */
void recover_one_disk(disk_array_t *disk_array, disk_t *broken_disk){

    if (broken_disk->type == data_disk || broken_disk->type == parity_disk){

      //
      char* newData = malloc(sizeof(char) * disk_array->block_size);
      char* buffer = malloc(sizeof(char) * disk_array->block_size);
      int starting;

      for (int p = 0; p < broken_disk->max_blocks; p++){

        bool worked;

        if (broken_disk != disk_array->disks[0]){
            worked = disk_read(disk_array->disks[0], p, buffer);
            int starting = 0;
        }
        else{
            worked = disk_read(disk_array->disks[1], p, buffer);
            int starting = 1;
            
        }
        
        
        if (worked == false){
          printf("Didn't work :(\n");
          return;
        }
      
        for (int j = 0; j < disk_array->block_size; j++){
      
          newData[j] = buffer[j];
        }  
          
      
        for (int i = 0;  i < disk_array->ndisks - 1; i++){ //Only -1 as we want to include pd
  
          if (i != starting && disk_array->disks[i]->working != false){
            disk_read(disk_array->disks[i], p, buffer); //Need to do something with true / false if read was unsuccessful
        
            for(int j = 0; j< disk_array->block_size; j++){
        
                newData[j] = newData[j] ^ buffer[j];
              
            }
          }
            
        }
      
        bool otherWorked = disk_write(broken_disk, p, newData);

      //
      
        }
      broken_disk->working = true;
      }

    
    

    else if (broken_disk->type == rs_disk){

        for (int block = 0; block < broken_disk->max_blocks; block++){
          update_rs_disk(disk_array, block);
        }
       
    }
        
      
          
    

      
      
     

  
}


void recover_two_disks(disk_array_t *disk_array, disk_t *broken_disk_one, disk_t *broken_disk_two){

    if ((broken_disk_one->type == parity_disk && broken_disk_two->type == rs_disk)|| (broken_disk_one->type == rs_disk && broken_disk_two->type == parity_disk)){ //Loss of Parity Drive and RS Drive

      recover_one_disk(disk_array, broken_disk_one);
      recover_one_disk(disk_array, broken_disk_two);


    }
    else if ((broken_disk_one->type == data_disk && broken_disk_two->type == rs_disk)){ //Loss of the RS drive and one data drive.
        recover_one_disk(disk_array, broken_disk_one);
        recover_one_disk(disk_array, broken_disk_two);
    }
    else if ((broken_disk_one->type == rs_disk && broken_disk_two->type == data_disk)){

      recover_one_disk(disk_array, broken_disk_two);
      recover_one_disk(disk_array, broken_disk_one);

    }

    //IF for Loss of the PD drive and one data drive.

    //IF Loss of two data drives.

}

