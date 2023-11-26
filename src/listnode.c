#include "listnode.h"
#include "filemanager.h"
#include "irootnode.h"
#include "doc.h"


listnode *createlistnode(int64_t count)
{
  listnode *list = malloc(sizeof(listnode));
  inode *ptr = inode_malloc(count);
  list->count = count;
  list->list = ptr;
  for (int64_t i = 0; i < list->count; ++i)
  {
    inode *tmp = list->list + i;
    tmp->empty = 1;
    tmp->id = i;
    tmp->parent = 0;
    tmp->size = 0;
    tmp->pos = 0;
  }

  // for (int64_t i = 0; i < 10; ++i) {
  //   inode *tmp = list->list + i;
  //   printf("%d ",tmp->empty);
  //   printf("%d ",tmp->id);
  //   printf("%d ",tmp->parent);
  //   printf("%d ",tmp->size);
  //   printf("%d \n",tmp->pos);
  // }
  return list;
}

listnode* readlistnode(file *file, int64_t count)
{
  uint64_t pos = ftello(file->F);
  fseek(file->F, sizeof(irootnode), SEEK_SET);
  listnode *l = malloc(sizeof(listnode));
  inode *ptr = inode_malloc(count);
  if (fread(ptr, sizeof(inode), count, file->F))
  {
    l->count = count;
    l->list = ptr;
  }
  fseek(file->F, pos, SEEK_SET);
  return l;
}

bool writelistnode(listnode *list, file *file)
{
  uint64_t pos = ftello(file->F);
  fseek(file->F, sizeof(irootnode), SEEK_SET);
  if (fwrite(list->list, sizeof(inode), list->count, file->F))
  {
    fseek(file->F, pos, SEEK_SET);
    // return true;
  }
  //   for (int64_t i = 0; i < 10; ++i) {
  //   inode *tmp = list->list + i;
  //   printf("%d ",tmp->empty);
  //   printf("%d ",tmp->id);
  //   printf("%d ",tmp->parent);
  //   printf("%d ",tmp->size);
  //   printf("%d \n",tmp->pos);
  // }
  return false;
}

// может переписать
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
    doc *d = createdoc(0);
    readoc(f, d);
    uint64_t pos = ftello(f->F);
    fseek(f->F, f->endpos, SEEK_SET);
    writedoc(f, d);
  }
  
  list->list = realloc(list->list, sizeof(inode) * precheercount);

  for (uint64_t i = list->count; i < precheercount; ++i)
  {
    inode* tmp = list->list + i;
    tmp->empty = 1;
    tmp->size = 0;
    tmp->id = i;
    tmp->parent = 0;
    tmp->pos = 0;
  }
  f->list->count = precheercount;
}
