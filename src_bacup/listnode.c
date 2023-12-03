#include "listnode.h"
#include "filemanager.h"
#include "doc.h"
#include "inode.h"


#include "stdlib.h"


listNode* createNode(uint64_t id, uint64_t size) {
    listNode* node = malloc(sizeof(listNode));
    if (node) {
        node->index = id;
        node->size = size;
        node->prev = NULL;
        node->next = NULL;
    }
    return node;
}

void destroyList(sortedList* list) {
    listNode* curr = list->front;
    while (curr) {
        listNode* next = curr->next;
        free(curr);
        curr = next;
    }
}

void insertNode(sortedList* list, listNode* node) {
    if (list->front) {
        listNode* curr = list->front;

        while (curr) {
            if ((node->size > curr->size) || (node->size == 0 && curr->size == 0)) {
                if (curr == list->front) {
                    list->front = node;
                } else {
                    curr->prev->next = node;
                }
                node->prev = curr->prev;
                node->next = curr;
                curr->prev = node;
                break;
            } else if (curr == list->back) {
                list->back = node;
                node->next = curr->next;
                node->prev = curr;
                curr->next = node;
                break;
            }
            curr = curr->next;
        }
    } else {
        list->front = node;
        list->back = node;
    }
}

listNode* popFront(sortedList* list) {
    if (list) {
        if (list->front) {
            listNode* front = list->front;
            list->front = list->front->next;
            if (list->front) {
                list->front->prev = NULL;
            } else {
                list->back = NULL;
            }
            return front;
        }
    }
    return NULL;
}

listNode* popBack(sortedList* list) {
    if (list) {
        if (list->back) {
            listNode* back = list->back;
            list->back = list->back->prev;
            if (list->back) {
                list->back->next = NULL;
            } else {
                list->front = NULL;
            }
            return back;
        }
    }
    return NULL;
}

bool removeNodeByIndexNumber(sortedList* list, uint64_t indexNumber) {
    listNode* curr = list->front;
    while (curr) {
        listNode* prev = curr->prev;
        listNode* next = curr->next;
        if (curr->index == indexNumber) {
            if (prev) {
                prev->next = next;
            }
            if (next) {
                next->prev = prev;
            }
            if (curr == list->front) {
                list->front = next;
            }
            if (curr == list->back) {
                list->back = prev;
            }
            free(curr->next);
            free(curr);
            return true;
        }
        curr = next;
    }
    return false;
}


bool readlistnode(file* file) {
  fseek(file->F, sizeof(head), SEEK_SET);
    uint64_t pos = ftello(file->F);
   for (uint64_t i = 0; i < file->header.count; i++) {
    fseek(file->F, pos, SEEK_SET);
    pos += sizeof(inode);
    inode node;
    if (!fread(&node, sizeof(inode), 1, file->F)) {
      return false;
    }
    // Заполняем список:
    if (node.status == OPEN) {
      insertNode(&file->list, createNode(0, node.id));
    } else if (node.status == DELETE) {
      insertNode(&file->list, createNode(node.size, node.id));
      }
    }
  return true;
}

bool writenewlistnode(file *file, uint64_t count)
{
  // uint64_t pos = ftello(file->F);
  // fseek(file->F, sizeof(irootnode), SEEK_SET);
  // listNode* l;
  // while ((l = popFront(&file->list))) {
  //   if (!fwrite(l->node, sizeof(inode), 1, file->F))
  //     return false;
  // }
  // fseek(file->F, pos, SEEK_SET);
  // return true;
  
  fseek(file->F, sizeof(head), SEEK_SET);
  inode node =  {.size = 0, .id = 0, .pos = 0, .status=OPEN};
  uint64_t written = 0;
    for (uint64_t i = 0; i < count; i++) {
      fwrite(&node, sizeof(inode), 1, file->F);
      insertNode(&file->list, createNode(i, node.size));
    }
    return written == count;
}


bool update_status_inode(file* file, uint64_t id, int8_t status) {
    bool written = 0;
    if (id < file->header.count) {
        uint64_t pos = ftello(file->F);
        fseek(file->F, (int64_t) (sizeof(head) + sizeof(inode) * id), SEEK_SET);
        written = fwrite(&status, sizeof(int8_t), 1, file->F);
        fseek(file->F, pos, SEEK_SET);
    };
    return written;
};

bool update_pos_inode(file* file, uint64_t id, uint64_t pos) {
    bool written = 0;
    if (id < file->header.count) {
        int64_t pos = ftello(file->F);
        fseek(file->F, (int64_t) (sizeof(head) + sizeof(inode) * id + sizeof(int8_t) + sizeof(uint64_t)), SEEK_SET);
        written = fwrite(&pos, sizeof(uint64_t), 1, file->F);
        fseek(file->F, pos, SEEK_SET);
    }
    return written;
};

bool update_size_inode(file* file, uint64_t id, uint64_t size) {
    bool written = 0;
    if (id < file->header.count) {
        int64_t pos = ftello(file->F);
        fseek(file->F, (int64_t) (sizeof(head) + sizeof(inode) * id + sizeof(uint8_t)), SEEK_SET);
        written = fwrite(&size, sizeof(uint64_t), 1, file->F);
        fseek(file->F, pos, SEEK_SET);
    }
    return written;
};


inode getinode(file* file, uint64_t index) {
    uint64_t pos = ftello(file->F);
    inode node = {.status=NOT_EXEC, .id=0,.pos=0, .size= 0};
    if (index < file->header.count) {
        fseek(file->F, (sizeof(head) + sizeof(inode) * index), SEEK_SET);
        fread(&node, sizeof(inode), 1, file->F);
    }
    fseek(file->F, pos, SEEK_CUR);
    return node;
}


// bool resize(file* file) {

//     uint64_t oldpos = ftello(file);
//     uint64_t readpos = sizeof(head) + sizeof(inode) * file->header.count;

//     fseek(file->F, readpos, SEEK_SET);

//     uint64_t size = 0;
//     uint64_t presecount = file->header.count;
//     uint8_t* buff = malloc(512);
    
//     while (presecount >= size) {
//         uint8_t* buff = malloc(512);
//         fread(buff, )

//     }
// }

// // может переписать
// bool resize(listnode *list, file *f)
// {
//   uint64_t precheercount = list->count << 1;
//   uint64_t size = 0;
//   uint64_t i = 0;
//   while (size <= precheercount * sizeof(inode))
//   {
//     size += list->list[i].size;
//     ++i;
//   }
//   for (uint64_t j = 0; j < i; ++j)
//   {
//     doc *d = malloc(sizeof(doc));
//     fseek(f->F, list->list[j].pos, SEEK_SET);
//     readoc(f, d);
//     uint64_t pos = ftello(f->F);
//     fseek(f->F, f->endpos, SEEK_SET);
//     writedoc(f, d);
//   }
  
//   list->list = realloc(list->list, sizeof(inode) * precheercount);

//   for (uint64_t i = list->count; i < precheercount; ++i)
//   {
//     inode* tmp = list->list + i;
//     tmp->status = OPEN;
//     tmp->size = 0;
//     tmp->id = i;
//     tmp->parent = 0;
//     tmp->pos = 0;
//   }
//   f->list->count = precheercount;
// }
