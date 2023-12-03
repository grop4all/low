
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


file* createfile(char* filename) {
  file* file = malloc(sizeof(file));
  file->F = fopen(filename, "w+b");
  file->header.endpos = 0;
  file->header.count = CDEFAULT;
  writeheader(file);
  createlistnode(file);
  writelistnode(file);
  file->header.endpos = sizeof(head) + sizeof(inode) * CDEFAULT;
  return file;
}

// где хранить endpos
file* readfile(file* file, char *filename) {
  file = malloc(sizeof(file));
  file->F = fopen(filename, "r+b");

  readheader(file);
  readlistnode(file);
  return file;
  // if (!file->F) return;
  // irootnode* irnode = readirootnode(file);
  // file->header.endpos = irnode->endpos;
  // file->list = readlistnode(file, irnode->count);

}

bool readheader(file* file) {
  uint64_t pos = ftell(file->F);
  fseek(file->F, 0, SEEK_SET);
  fread(&file->header.count, sizeof(uint64_t), 1, file->F);
  fread(&file->header.endpos, sizeof(uint64_t), 1, file->F);
  fseek(file->F, pos, SEEK_SET);
}


bool writeheader(file* file) {
  uint64_t pos = ftell(file->F);
  fseek(file->F, 0, SEEK_SET);
  fwrite(&file->header.count, sizeof(uint64_t), 1, file->F);
  fwrite(&file->header.endpos, sizeof(uint64_t), 1, file->F);
  fseek(file->F, pos, SEEK_SET);
}


void closefile(file* file) {
  free(file);
  fclose(file->F);
}



