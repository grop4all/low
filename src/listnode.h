#ifndef LLP_SRC_LISTNODE_H_
#define LLP_SRC_LISTNODE_H_

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

#include "inode.h"
#include "filemanager.h"
typedef struct file file;


typedef struct listnode {
  uint64_t count;
  inode* list;
} listnode;


listnode* createlistnode(int64_t count);
listnode* readlistnode(file* f, int64_t count);
bool writelistnode(listnode* list, file* file);
bool resize(listnode* list1, file* f);



#endif //LLP_SRC_LISTNODE_H_
