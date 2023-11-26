
/*
 * тут надо разрулить создание нового файла
 *
 * и придумать дефолтное коллличество inode и т.д.
 */



//void initialize(file* file) {
//  irootnode* irootnode = loadirootnode(file);
//  listnode* list = readlistnode(f, irootnode->count);
//
//
//}


#include "listnode.h"
#include "irootnode.h"

file* createfile(char* filename) {
  file* f = malloc(sizeof(file));
  f->F = fopen(filename, "w+b");
  f->endpos = 0;
  irootnode* irnode = createirootnode(CDEFAULT);
  writeirootnode(f, irnode);
  listnode* list = createlistnode(CDEFAULT);
  writelistnode(list, f);
  f->list = list;
  f->endpos = sizeof(irootnode) + sizeof(inode) * CDEFAULT;
  return f;
}

// где хранить endpos
void readfile(file* file, char *filename) {
  file->F = fopen(filename, "r+b");

  if (!file->F) return;
  irootnode* irnode = readirootnode(file);
  file->endpos = irnode->endpos;
  file->list = readlistnode(file, irnode->count);

}



void closefile(file* file) {
  updateirootnode(file);
  fclose(file->F);
}



