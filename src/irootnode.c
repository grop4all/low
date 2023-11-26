#include "irootnode.h"
#include "filemanager.h"

irootnode* createirootnode(uint64_t count) {
  irootnode* irnode = malloc(sizeof(irootnode));
  irnode->count = count;
  irnode->endpos = 0;
  return irnode;
}


void writeirootnode(file* file, irootnode* ir) {
  uint64_t pos = ftello(file->F);
  fseek(file->F, 0, SEEK_SET);
  fwrite(ir, sizeof(irootnode), 1, file->F);
  fseek(file->F, pos, SEEK_SET);
}


void updateirootnode(file* file) {
  irootnode* irnode = createirootnode(file->list->count);
  irnode->endpos = file->endpos;
  writeirootnode(file, irnode);
}

irootnode* readirootnode(file* file) {
  uint64_t pos = ftello(file->F);
  fseek(file->F, 0, SEEK_SET);
  irootnode* irootnode_ = createirootnode(0);
  fread(irootnode_, sizeof(irootnode), 1,file->F);
  fseek(file->F, pos, SEEK_SET);
  return irootnode_ ? irootnode_ : NULL;
}

