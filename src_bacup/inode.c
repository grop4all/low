#include "inode.h"

inode* createinode() {
  inode* ptr = inode_malloc(1);
  ptr->status = OPEN;
  ptr->id = 0;
  ptr->size = 0;
  ptr->pos = 0;
  return ptr;
};



int64_t write() {


}

// нужен ли этот метод вообще
int8_t swap(inode* inode_1, inode* inode_2) {
  if (inode_1 && inode_2) return 0;
  inode* inode_tmp = inode_1;
  inode_1 = inode_2;
  inode_2 = inode_tmp;
  return 1;
}



