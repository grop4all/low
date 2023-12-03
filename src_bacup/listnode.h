#ifndef LLP_SRC_LISTNODE_H_
#define LLP_SRC_LISTNODE_H_

#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

#include "inode.h"

typedef struct file file;

#include <stdint.h>
#include "stdbool.h"
#include "inode.h"

/* Структура для узла списка. */
typedef struct listNode {
  uint64_t size; // размер документа
  uint64_t index; // номер индекса
  struct listNode* prev; // указатель на предыдущий элемент списка
  struct listNode* next; // указатель на следующий элемент списка
} listNode;

/* Структура для отсортированного списка индексов свободных мест в файле. */
typedef struct sortedList {
    listNode* front; // указатель на первый элемент списка
    listNode* back; // указатель на последний элемент списка
} sortedList;

/* Функция для создания узла. */
listNode* createNode(uint64_t id, uint64_t size);

/* Функция для уничтожения списка. */
void destroyList(sortedList* list);

/* Функция для вставки узла. */
void insertNode(sortedList* list, listNode* node);

/* Функция для извлечения первого узла из списка. */
listNode* popFront(sortedList* list);

/* Функция для извлечения последнего узла из списка. */
listNode* popBack(sortedList* list);

/* Функция для удаления из списка узла, привязанного к определённому документу. */
bool removeNodeByIndexNumber(sortedList* list, uint64_t indexNumber);


bool writenewlistnode(file *file, uint64_t count);
bool readlistnode(file* file);
bool update_pos_inode(file* file, uint64_t id, uint64_t pos);
bool update_status_inode(file* file, uint64_t id, int8_t status);
inode getinode(file* file, uint64_t index);
bool update_size_inode(file* file, uint64_t id, uint64_t pos);
// typedef struct listnode {
//   uint64_t count;
//   inode* list;
// } listnode;


// listnode* createlistnode(int64_t count);
// listnode* readlistnode(file* f, int64_t count);
// bool writelistnode(listnode* list, file* file);
// bool resize(listnode* list1, file* f);



#endif //LLP_SRC_LISTNODE_H_
