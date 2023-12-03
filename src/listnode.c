#include "listnode.h"
#include "filemanager.h"
#include "doc.h"
#include "inode.h"


#include "stdlib.h"





bool createlistnode(file* file) {
    file->list = malloc(sizeof(listnode));
    if ((file->list->list = inode_malloc(file->header.count)))
    {
        for (uint64_t i = 0; i < file->header.count; ++i) {
            inode* tmp = file->list->list + i;
            tmp->status = OPEN;
            tmp->size = 0;
            tmp->id = i;
            tmp->parent = 0;
            tmp->pos = 0;
        }
    }
}




bool readlistnode(file *file)
{
  uint64_t pos = ftello(file->F);
  fseek(file->F, sizeof(head), SEEK_SET);
  listnode *l = malloc(sizeof(listnode));
  inode *ptr = inode_malloc(file->header.count);
  if (fread(ptr, sizeof(inode), file->header.count, file->F))
  {
    l->count = file->header.count;
    l->list = ptr;
    file->list = l;
    fseek(file->F, pos, SEEK_SET);
    return true;
  }
  fseek(file->F, pos, SEEK_SET);
  return false;
}

bool writelistnode(file *file)
{
  uint64_t pos = ftello(file->F);
  fseek(file->F, sizeof(head), SEEK_SET);
  if (fwrite(file->list->list, sizeof(inode), file->header.count, file->F))
  {
    fseek(file->F, pos, SEEK_SET);
    return true;
  }
  return false;
}

// // может переписать
bool resize(listnode *list, file *f)
{
  uint64_t precheercount = list->count << 1;
  uint64_t size = 0;
  uint64_t i = 0;
  while (size <= precheercount * sizeof(inode))
  {
    size += list->list[i].size;
    ++i;
  }
  for (uint64_t j = 0; j < i; ++j)
  {
    doc *d = malloc(sizeof(doc));
    fseek(f->F, list->list[j].pos, SEEK_SET);
    readoc(f, d);
    uint64_t pos = ftello(f->F);
    fseek(f->F, f->header.endpos, SEEK_SET);
    writedoc(f, d);
  }
  
  list->list = realloc(list->list, sizeof(inode) * precheercount);

  for (uint64_t i = list->count; i < precheercount; ++i)
  {
    inode* tmp = list->list + i;
    tmp->status = OPEN;
    tmp->size = 0;
    tmp->id = i;
    tmp->parent = 0;
    tmp->pos = 0;
  }
  f->list->count = precheercount;
}
