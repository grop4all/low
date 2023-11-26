#ifndef LLP_SRC_DOC_H_
#define LLP_SRC_DOC_H_

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"



#include "filemanager.h"


typedef enum elementType {
  UNDEFINE = 0,
  TYPE_INT = 1, // для int32_t
  TYPE_DOUBLE = 2, // для double
  TYPE_BOOLEAN = 3, // для boolean (uint8_t)
  TYPE_STRING = 4, // для строки
} elementType;


typedef struct string {
  uint32_t len;
  char* data;
} string;

typedef union elem {
  double double_;
  int32_t int_;
  uint8_t char_;
  uint8_t bool_;
  string string_;
} elem;

typedef struct doc {
  uint64_t count;
  elementType* typelist; // fread(,int8,count,)
  elem* elemlist;
} doc;


bool addelem (doc* d, elementType type, elem elem);
doc* createdoc(uint64_t count);
bool readoc (file* f, doc* d);
uint64_t writedoc(file* f, doc* d);
void printdoc(doc* d);


#endif //LLP_SRC_DOC_H_
