//
// Created by nikita on 12.11.23.
//

#ifndef LLP_SRC_IROOTNODE_H_
#define LLP_SRC_IROOTNODE_H_

#include "stdlib.h"
#include "stdio.h"
#include "stdio.h"
#include "stdint.h"



typedef struct irootnode {
  uint64_t count;
  uint64_t endpos;
} irootnode;

#include "filemanager.h"

irootnode* readirootnode(file* file);
irootnode* createirootnode(uint64_t count);
void updateirootnode(file* file);
void writeirootnode(file* file, irootnode* ir);


#endif //LLP_SRC_IROOTNODE_H_
