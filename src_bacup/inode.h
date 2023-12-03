//
// Created by nikita on 12.11.23.
//

#ifndef LLP_SRC_INODE_H_
#define LLP_SRC_INODE_H_
#include <stdlib.h>


#include "stdint.h"

enum st {
  CLOSE = 0,
  OPEN,
  DELETE,
  NOT_EXEC,
};
#define inode_malloc(n) malloc(sizeof(inode) * n)


typedef struct inode  {
  int8_t status;
  uint64_t size;
  uint64_t pos;
  uint64_t id;
} inode;


inode* createinode();




#endif //LLP_SRC_INODE_H_