#include "doc.h"
#include "opetarion.h"
#include "filemanager.h"


//  todo:
//  не хватате вывода в консоль

uint64_t get_idoc(file* file) {
  uint64_t pos = ftell(file->F);
  meta meta;
  fread(&meta, sizeof(meta), 1, file->F);
  fseek(file->F, pos, SEEK_SET);
  return meta.id;
}

uint64_t get_sizedoc(file* file) {
  uint64_t offset = ftell(file->F);
  meta meta;
  fread(&meta, sizeof(meta), 1, file->F);
  fseek(file->F, offset, SEEK_SET);
  return meta.size;
}

doc* createdoc(uint64_t count) {
  doc* d = malloc(sizeof(doc));
  d->meta.child = -1;
  d->meta.count = count;
  d->typelist = malloc(sizeof(int8_t) * count);
  for (uint64_t i = 0; i < count ; ++i) d->typelist[i] = UNDEFINE;
  d->elemlist = malloc(sizeof(elem) * count);
  if (d && d->elemlist && d->typelist) return d;
  return NULL;
}

void destroydoc(doc* d) {
  if (d) {
    if (d->elemlist) free(d->elemlist);
    if (d->typelist) free(d->typelist);
  }
  free(d);
}



bool addelem(doc* d, elementType type, elem elem) {
  if (d->meta.count != 0) { 
    uint64_t i = 0;
    for (;d->typelist[i] != UNDEFINE && i < d->meta.count; ++i);
    if (type == TYPE_STRING) {
      d->elemlist[i].string_.len = elem.string_.len;
      d->elemlist[i].string_.data = elem.string_.data;
      d->typelist[i] = TYPE_STRING;
    }
    d->elemlist[i] = elem;
    d->typelist[i] = type;
    
  }
  return false;
}


uint64_t offsetelem (doc *d, uint64_t index) {
  uint64_t offset = 0;
  offset += d->meta.count * sizeof(elementType);
  for (uint64_t i = 0; i < index; ++i) {
    switch (d->typelist[i])
    {
      case TYPE_INT: 
        offset +=  sizeof(int32_t);
        break;
      case TYPE_DOUBLE:
        offset += sizeof(double);
        break;
      case TYPE_BOOLEAN:
        offset += sizeof(uint8_t);
        break;
      case TYPE_STRING:
        offset += sizeof(uint32_t);
        offset += d->elemlist[i].string_.len * sizeof(char);
        break;
      }
  }
  return offset;
}

// bool updatedoc(file* file, uint64_t indexdoc, uint64_t indexelem, elementType type, elem value) {
  
//   uint64_t oldpos = ftello(file->F);
//   if (file->list->list[indexdoc].status == CLOSE ||
//       file->list->list[indexdoc].status ==  DELETE) return false;
//   uint64_t pos = file->list->list[indexdoc].pos;
//   fseek(file->F, pos, SEEK_SET);
//   doc* d = malloc(sizeof(doc));
//   readoc(file, d);
//   if (d->typelist[indexelem] == UNDEFINE) return false;
//   pos += offsetelem(d, indexelem);
//   fseek(file->F, pos, SEEK_SET);
//   switch (type)
//     {
//       case TYPE_INT: 
//         fwrite(&value.int_, sizeof(int32_t), 1, file->F);
//         break;
//       case TYPE_DOUBLE:
//         fread(&value.double_, sizeof(double), 1, file->F);
//         break;
//       case TYPE_BOOLEAN:
//         fread(&value.bool_, sizeof(uint8_t), 1, file->F);
//         break;
//       case TYPE_STRING:
//         updatestring(file, indexdoc, indexelem, d, value);
//         break;
//     }
//   fseek(file->F, oldpos, SEEK_SET);
// }



// bool updatestring(file* file, uint64_t indexdoc, uint64_t indexelem, doc *d, elem value) {

//   uint64_t pos = offsetelem(d, indexelem);
//   if (d->elemlist[indexelem].string_.len > value.string_.len)
//   {
//     fseek(file->F, pos, SEEK_SET);
//     fwrite(&value.string_.len, sizeof(uint32_t), 1, file->F);
//     fwrite(value.string_.data, sizeof(char), d->elemlist->string_.len, file->F);
//   }
//   else {
//     fseek(file->F, file->endpos, SEEK_SET);
//     free(d->elemlist[indexelem].string_.data);
//     d->elemlist[indexdoc].string_.len = value.string_.len;
//     d->elemlist[indexdoc].string_.data = value.string_.data;
//     insert(file, d);
//     delete(file, indexdoc);
//   }

// }





doc* readoc(file* file, uint64_t index) {

  uint64_t pos = ftello(file->F);
  inode node =  getinode(file, index);
  fseek(file->F, node.pos, SEEK_SET);
  meta meta;
  fread(&meta, sizeof(struct meta), 1, file->F);

  doc* d  = createdoc(meta.count);

  fread(d->typelist, sizeof(int8_t), d->meta.count, file->F);
  for (uint64_t i = 0; i < d->meta.count; ++i) {
    switch (d->typelist[i])
    {
      case TYPE_INT:
        fread(&d->elemlist[i].int_, sizeof(int32_t), 1, file->F);
        break;
      case TYPE_DOUBLE:
        fread(&d->elemlist[i].double_, sizeof(double), 1, file->F);
        break;
      case TYPE_BOOLEAN:
        fread(&d->elemlist[i].bool_, sizeof(uint8_t), 1, file->F);
        break;
      case TYPE_STRING:
        fread(&d->elemlist[i].string_.len, sizeof(uint32_t), 1, file->F);
        if (d->elemlist[i].string_.len)
        {
        d->elemlist->string_.data = malloc(d->elemlist->string_.len);
        fread(d->elemlist[i].string_.data, sizeof(char), d->elemlist->string_.len, file->F);
        }
        break;
    }
  }
  fseek(file->F, pos, SEEK_SET);
  return d;
}


uint64_t writedoc(file* file, doc* d, uint64_t indexchild) {

  // pos уже установлен
  uint64_t offset = 0;
  uint64_t pos = ftello(file->F);
  uint64_t newSize = 0;
  d->meta.size = calcdoc(d);
  if (!file->list.front && !moveFirstDocuments(file)) {
    return 0;
  }
  int64_t diff = (int64_t) file->list.front->size - (int64_t) d->meta.size;

  if (!file->list.front && !moveFirstDocuments(file)) {
    return 0;
  }
  if (diff >= 0) {
    inode node = getinode(file, file->list.front->index);
    if (node.status != DELETE || 
        !update_status_inode(file, file->list.front->index, CLOSE)) {
            return 0;
        }
        // Заполняем заголовок документа:
        d->meta.id = file->list.front->index;
        // header.id.offset = index.offset;
        // Если дырка больше, чем надо, записываем TYPE_NOT_EXIST в том месте, где будет заканчиваться документ:
        // if (diff) {
        //     uint8_t startOfUnusedSpaceMark = TYPE_NOT_EXIST;
        //     fseeko64(file->f, index.offset + (int64_t) header.size, SEEK_SET);
        //     if (!fwrite(&startOfUnusedSpaceMark, sizeof(uint8_t), 1, file->f)) {
        //         return not_present_uint64_t();
        //     }
        //     newSize = file->list.front->size;
        // }
        free(popFront(&file->list));
    } else {
      if (file->list.back->size != 0 && !moveFirstDocuments(file) ||
            !update_status_inode(file, file->list.back->index, CLOSE) ||
            !update_pos_inode(file, file->list.back->index, file->header.endpos) ||
            !update_size_inode(file, file->list.back->index, d->meta.size)){
            return 0;
        }
        d->meta.id = file->list.back->index;
        free(popBack(&file->list));
        // Обновляем размер файла
        file->header.endpos += (int64_t) d->meta.size;
        writeheader(file);
    }
    inode node = getinode(file, d->meta.id);

    fseek(file->F, node.pos, SEEK_SET);
    fwrite(&d->meta, sizeof(struct meta), 1, file->F);
    fwrite(d->typelist, sizeof(int8_t), d->meta.count, file->F);
    for (int64_t i = 0; i < d->meta.count; ++i) {
      switch (d->typelist[i]) {
        case TYPE_INT:
          offset += fwrite(&d->elemlist[i].int_, sizeof(int32_t), 1, file->F) * sizeof(int32_t);
          break;
        case TYPE_DOUBLE:
          offset += fwrite(&d->elemlist[i].double_, sizeof(double), 1, file->F) * sizeof(double);
          break;
        case TYPE_BOOLEAN:
          offset += fwrite(&d->elemlist[i].bool_, sizeof(uint8_t), 1, file->F) * sizeof(uint8_t);
          break;
        case TYPE_STRING:
          offset += fwrite(&d->elemlist[i].string_.len, sizeof(uint32_t), 1, file->F) * sizeof(uint32_t);
          offset += fwrite(d->elemlist[i].string_.data, sizeof(char), d->elemlist->string_.len, file->F) * sizeof(char);
            break;
      }
    }
    fseek(file->F, pos, SEEK_SET);
    return offset;
}




// bool getdoc(file* file, uint64_t index, doc* d) {

//   uint64_t pos = ftello(file->F);
//   if (file->list->list[index].status != OPEN) {
//     fseek(file->F, file->list->list[index].pos, SEEK_SET);
//     readoc(file, d);
//     return true;
//   }
//   fseek(file->F, pos, SEEK_SET);
//   return false;
// }

void printdoc(doc* d) {
  printf("{\n");
  for (int64_t i = 0; i < d->meta.count ; ++i) {
    switch (d->typelist[i])
    {
      case TYPE_INT:
        printf("\t TYPE_INT");
        printf("\t %d", d->elemlist[i].int_);
        printf("\n");
        break;
      case TYPE_DOUBLE:
        printf("\t TYPE_DOUBLE");
        printf("\t %f", d->elemlist[i].double_);
        printf("\n");
        break;
      case TYPE_BOOLEAN:
        printf("\t TYPE_BOOLEAN");
        printf("\t %u", d->elemlist[i].bool_);
        printf("\n");
        break;
      case TYPE_STRING:
        printf("\t TYPE_STRING");
        printf("\t %s", d->elemlist[i].string_.data);
        printf("\n");
        break;
    }
  }
  printf("\n}");
}



uint64_t calcdoc(doc* d) {
  uint64_t count = 0;
  count += sizeof(struct meta);
  count += d->meta.count * sizeof(int8_t);
  for (int64_t i = 0; i < d->meta.count; ++i) {
    switch (d->typelist[i]) {
      case TYPE_INT:
        count += sizeof(int32_t);
        break;
      case TYPE_DOUBLE:
        count += sizeof(double);
        break;
      case TYPE_BOOLEAN:
        count += sizeof(int8_t);
        break;
      case TYPE_STRING:
        count += sizeof(uint32_t);
        count += d->elemlist[i].string_.len * sizeof(char);
        break;
    }
  }
  return count;

}




