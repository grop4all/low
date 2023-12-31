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

#define CDEFAULT 10000

typedef struct listnode listnode;

typedef struct head {
  uint64_t endpos;
  uint64_t count;
} head;

typedef struct file {
  head header;
  FILE* F;
  listnode* list;
} file;

bool writeheader(file* file);
bool readheader(file* file);
file* createfile(char* filename);
file* readfile(file* file, char *filename);
void closefile(file* file);

#endif //LLP_SRC_FILEMANAGER_H_
