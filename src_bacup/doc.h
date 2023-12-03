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

typedef struct meta {
  uint64_t id;
  uint64_t size;
  int64_t child;
  uint64_t count;
} meta;

typedef struct doc {
  meta meta;
  int8_t* typelist;
  elem* elemlist;
} doc;

uint64_t get_idoc(file* file);
uint64_t get_sizedoc(file* file);

bool addelem (doc* d, elementType type, elem elem);
doc* createdoc(uint64_t count);
doc* readoc(file* file, uint64_t index);
uint64_t writedoc(file* f, doc* d, uint64_t);
void printdoc(doc* d);
bool updatestring(file* file, uint64_t indexdoc, uint64_t indexelem, doc *d, elem value);
uint64_t calcdoc(doc* d);
bool getdoc(file* file, uint64_t index, doc* d);

#endif //LLP_SRC_DOC_H_
