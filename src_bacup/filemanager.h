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
#define BUF_SIZE 500000000

typedef struct sortedList sortedList;

typedef struct head {
  uint64_t firstpos;
  uint64_t endpos;
  uint64_t count;
} head;

typedef struct file {
  head header;
  FILE* F;
  sortedList list;
} file;

bool readheader(file* file);
bool moveFirstDocuments(file* file);
bool moveData(file* file, int64_t* oldPos, int64_t* newPos, uint64_t size);
bool writeheader(file* file);
file* createfile(char* filename);
bool writenewlistnode(file *file, uint64_t count);
file* readfile(char *filename);
void closefile(file* file);

#endif //LLP_SRC_FILEMANAGER_H_
