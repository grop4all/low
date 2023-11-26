//
// Created by nikita on 12.11.23.
//

#ifndef LLP_SRC_INODE_H_
#define LLP_SRC_INODE_H_
#include <stdlib.h>


#include "stdint.h"


#define inode_malloc(n) malloc(sizeof(inode) * n)


typedef struct inode  {
  uint8_t empty;
  int64_t id;
  int64_t parent;
  int64_t pos;
  int64_t size;
} inode;







#endif //LLP_SRC_INODE_H_