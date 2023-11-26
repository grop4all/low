//
// Created by nikita on 12.11.23.
//

  /*
   *  todo:
   *  writeheader for auth file on this format
   *  add checksum
   *
   */

#ifndef LLP_SRC_FILEMANAGER_H_
#define LLP_SRC_FILEMANAGER_H_



#include "listnode.h"

#define CDEFAULT 1048576

typedef struct listnode listnode;


typedef struct file {
  uint64_t endpos;
  FILE* F;
  listnode* list;
} file;


file* createfile(char* filename);
void readfile(file* file, char *filename);
void closefile(file* file);

#endif //LLP_SRC_FILEMANAGER_H_
