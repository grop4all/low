//
// Created by nikita on 14.11.23.
//

#ifndef LLP_SRC_OPETARION_H_
#define LLP_SRC_OPETARION_H_


#include "float.h"
#include <stdbool.h>

#include "filemanager.h"
#include "irootnode.h"
#include "listnode.h"
#include "doc.h"


#define DEFAULT 1048576



void insert(file* file, doc* doc);
bool delete(file* file, uint64_t index);
bool update(file* file, uint64_t index, doc* doc);
#endif //LLP_SRC_OPETARION_H_
