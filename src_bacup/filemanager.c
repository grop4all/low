
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

#include "filemanager.h"
#include "doc.h"
#include "listnode.h"

file* createfile(char* filename) {
  file* file = malloc(sizeof(struct file));
  file->F = fopen(filename, "w+b");
  file->header.count = CDEFAULT;
  file->header.endpos = sizeof(head) + sizeof(inode) * CDEFAULT;
  file->header.firstpos = sizeof(head) + sizeof(inode) * CDEFAULT;
  file->list = (sortedList) {NULL, NULL};
  writeheader(file); 
  writenewlistnode(file, CDEFAULT);
  return file;
}

// где хранить endpos
file* readfile(char *filename) {
  file* file = malloc(sizeof(struct file));
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
  return true;
}


bool writeheader(file* file) {
  uint64_t pos = ftell(file->F);
  fseek(file->F, 0, SEEK_SET);
  fwrite(&file->header.count, sizeof(uint64_t), 1, file->F);
  fwrite(&file->header.endpos, sizeof(uint64_t), 1, file->F);
  fseek(file->F, pos, SEEK_SET);
  return true;
}

void closefile(file* file) {
    if (file) {
        if (file->F) {
            fclose(file->F);
        }
        destroyList(&file->list);
        free(file);
    }
}
// 
bool moveData(file* file, int64_t* oldPos, int64_t* newPos, uint64_t size) {
    while (size) {
        // Определяем размер буфера и аллоцируем его:
        int64_t bufSize;
        if (size > BUF_SIZE) {
            bufSize = BUF_SIZE;
        } else {
            bufSize = (int64_t) size;
        }
        size -= bufSize;
        uint8_t* buf = malloc(bufSize);

        // Перемещаемся на прошлый адрес и заполняем буфер:
        fseek(file->F, *oldPos, SEEK_SET);
        if (!fread(buf, bufSize, 1, file->F)) {
            free(buf);
            return false;
        
        *oldPos += bufSize;

        // Перемещаемся на новый адрес и пишем из буфера:
        fseek(file->F, *newPos, SEEK_SET);
        if (!fwrite(buf, bufSize, 1, file->F)) {
            free(buf);
            return false;
        }
        *newPos += bufSize;
        free(buf);
    }
    }
    return true;
}

bool moveFirstDocuments(file* file) {
    // Смещаемся к началу документов:
    int64_t newPos;
    int64_t oldPos = (int64_t) (sizeof(head) + sizeof(inode) * file->header.count +
                                file->header.firstpos);
    /* Перемещаем документы, пока места недостаточно. Изначально доступно file->header.firstDocumentOffset, поскольку
     * перед документами могут быть неиспользуемые байты: */
    int64_t neededSpace = sizeof(inode) * CDEFAULT;
    int64_t availableSpace = file->header.firstpos;
    while (availableSpace < neededSpace) {
        // Считываем заголовок документа:
        fseek(file->F, oldPos, SEEK_SET);
        uint64_t idoc = get_idoc(file);
        uint64_t sizedoc = get_sizedoc(file);
        // Считываем индекс, привязанный к документу:
        uint64_t newHeaderSize = 0;
        inode index = getinode(file, idoc);
        if (index.status == DELETE) {
            // Если наш документ - дырка, удаляем его из списка индексов и делаем INDEX_NEW.
            if (!removeNodeByIndexNumber(&file->list, idoc) ||
                !update_status_inode(file, idoc, OPEN) || !update_pos_inode(file, idoc, 0) ||
                !update_size_inode(file, idoc, 0)){
                return false;
            }
            insertNode(&file->list, createNode(idoc, 0));
            oldPos += (int64_t) sizedoc;
        } else if (index.status == CLOSE) {
            /* Если документ живой, то его нужно переместить.
             * Если есть подходящая дырка, в которую можно переместить документ, то нужно сделать индекс дырки новым
             * (flag = INDEX_NEW, offset = 0), а прошлое смещение дырки записать в индекс переносимого блока.
             * Если подходящих дырок нет (или список пустой), то нужно перемещать документ в конец файла. */
            if (file->list.front && file->list.front->size >= sizedoc) {
                inode gapIndex = getinode(file, file->list.front->index);
                if (gapIndex.status != DELETE ||
                    !update_status_inode(file, file->list.front->index, OPEN) ||
                    !update_pos_inode(file, file->list.front->index, 0) || 
                    !update_size_inode(file, file->list.front->index, 0)) {
                    return false;
                }
                newPos = gapIndex.pos;
                newHeaderSize = file->list.front->size;
                // Записываем дырку обратно в список, но уже с размером 0:
                listNode* node = popFront(&file->list);
                node->size = 0;
                insertNode(&file->list, node);
                // Если дырка больше, чем надо, записываем TYPE_NOT_EXIST в том месте, где будет заканчиваться документ:
                // if (newHeaderSize > sizedoc) {
                //     uint8_t startOfUnusedSpaceMark = TYPE_NOT_EXIST;
                //     fseeko64(file->F, newPos + (int64_t) header.size, SEEK_SET);
                //     if (!fwrite(&startOfUnusedSpaceMark, sizeof(uint8_t), 1, file->F)) {
                //         return false;
                //     }
                // }
            } else {
                newPos = file->header.endpos;
                // Обновляем размер файла:
                file->header.endpos += (int64_t) sizedoc;
                if (!writeheader(file)) {
                    return false;
                }
            }
            // Перемещаем документ, обновляем смещение в его индексе:
            if (!update_pos_inode(file, idoc, newPos) ||
                !moveData(file, &oldPos, &newPos, sizedoc)) {
                return false;
            }
        } else {
            return false;
        }
        availableSpace += (int64_t) sizedoc; // возможно переполнение, если ZGDB_DEFAULT_INDEX_CAPACITY будет слишком большим!
        // Смещаемся к началу нового места документа и обновляем его заголовок, если он был перемещён в дырку:
        // if (newHeaderSize) {
        //     fseek(file->F, newPos - (int64_t) sizedoc, SEEK_SET);
        //     header.size = newHeaderSize;
        //     if (!fwrite(&header, sizeof(documentHeader), 1, file->F)) {
        //         return false;
        //     }
        // }
    }
    // Записываем новые индексы и сохраняем остаток места:
    file->header.firstpos = availableSpace % sizeof(inode);
    if (!writenewlistnode(file, availableSpace / sizeof(inode)) || !writeheader(file)) {
        return false;
    }
    return true;
}
// void closefile(file* file) {
//   updateirootnode(file);
//   fclose(file->F);
// }



