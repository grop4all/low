//
// Created by nikita on 14.11.23.
//


/*
 * написать обраное дерево
 *
 *
 * еще надо написать вывод документов << 1
 *
 *
 */

#include "opetarion.h"
#include "doc.h"
#include "filemanager.h"



void insert(file* file, doc* doc) {

  uint64_t oldpos = ftello(file->F);  
  listnode* list = file->list;
  uint64_t i = 0;
  uint64_t offset = 0;
  for (; i < list->count && file->list->list[i].status == OPEN; ++i);
  if (list->count == i)
  {
    resize(list, file);
  }

  // if (list->list[i].size != 0 && list->list[i].size <= calcdoc(doc)) { 
    
  // }
  fseek(file->F, file->header.endpos, SEEK_SET); // до delete update endpos = sec_end
  if ((offset = writedoc(file, doc))) {

    inode* tmp = list->list + i;
    tmp->pos = file->header.endpos;
    file->header.endpos += offset;
    tmp->status = 0;
    tmp->size = offset;
    // tmp->parent = doc->parent;
  }
  fseek(file->F, oldpos, SEEK_SET);
}


bool delete(file* file, uint64_t index) {

  listnode* list = file->list;
  if (index >= list->count) return false;
  list->list[index].status = 1;


  // uint64_t oldpos = ftello(file->F);
  // listnode* list = file->list;
  // if (index >= list->count) return false;

  // void* tmp = malloc(DEFAULT);
  // list->list[index].empty = 1;

  // uint64_t offset = list->list[index].size;

  // // uint64_t dpos = list->list[index].pos;
  // uint64_t rpos = offset + list->list[index].pos;
  // uint64_t wrpos = list->list[index].pos;


  // uint64_t rrb = 0; // real read bit
  // fseek(file->F, rpos, SEEK_SET);

  // while ((rrb = fread(tmp, 1, DEFAULT, file->F)))
  // {
  //   rpos += rrb;
  //   fseek(file->F, wrpos, SEEK_SET);
  //   wrpos += fwrite(tmp, 1, rrb, file->F);
  //   fseek(file->F, rpos, SEEK_SET);
  //   rrb = 0;
  // }
  // fseek(file->F, oldpos, SEEK_SET);
  // file->endpos = wrpos;
  // return rpos - offset == wrpos;
}


bool update(file* file, uint64_t index, doc* doc) {
  listnode* list = file->list;
  uint64_t i = 0;
  for (; i < list->count && i != index; ++i);
  if (i > list->count) return false;

  delete(file, index);
  insert(file, doc);
}