#include "doc.h"
#include "opetarion.h"
#include "filemanager.h"


//  todo:
//  не хватате вывода в консоль


doc* createdoc() {
  doc* d = malloc(sizeof(doc));
  d->count = 0;
  if (d) return d;
  return NULL;
}

void destroydoc(doc* d) {
  if (d) {
    if (d->elemlist) free(d->elemlist);
    if (d->typelist) free(d->typelist);
  }
  free(d);
}




bool addelem (doc* d, elementType type, elem elem) {
  if (d->elemlist && d->typelist) { 
    uint64_t i = 0;
    for (;d->typelist[i] != UNDEFINE && i < d->count; ++i);

    if (d->count == i) {
      d->count <<= 1;
      d->elemlist = realloc(d->elemlist, d->count);
      d->typelist = realloc(d->typelist, d->count);
    };
      if (type == TYPE_STRING) {
      d->elemlist[i].string_.len = elem.string_.len;
      d->elemlist[i].string_.data = elem.string_.data;
    }
    d->elemlist[i] = elem;
  } else {
    d->elemlist = calloc(10, sizeof(elementType));
    d->typelist = calloc(10, sizeof(elem));
    d->typelist[0] = type;
    if (type == TYPE_STRING) {
      d->elemlist[0].string_.len = elem.string_.len;
      d->elemlist[0].string_.data = elem.string_.data;
    }
    d->elemlist[0] = elem;
  }
}


uint64_t offsetelem (doc *d, uint64_t index) {
  uint64_t offset = 0;
  offset += d->count * sizeof(elementType);
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

bool updatedoc(file* file, uint64_t indexdoc, uint64_t indexelem, elementType type, elem value) {
  
  uint64_t oldpos = ftello(file->F);
  if (file->list->list[indexdoc].status == CLOSE ||
      file->list->list[indexdoc].status ==  DELETE) return false;
  uint64_t pos = file->list->list[indexdoc].pos;
  fseek(file->F, pos, SEEK_SET);
  doc* d = malloc(sizeof(doc));
  readoc(file, d);
  if (d->typelist[indexelem] == UNDEFINE) return false;
  pos += offsetelem(d, indexelem);
  fseek(file->F, pos, SEEK_SET);
  switch (type)
    {
      case TYPE_INT: 
        fwrite(&value.int_, sizeof(int32_t), 1, file->F);
        break;
      case TYPE_DOUBLE:
        fread(&value.double_, sizeof(double), 1, file->F);
        break;
      case TYPE_BOOLEAN:
        fread(&value.bool_, sizeof(uint8_t), 1, file->F);
        break;
      case TYPE_STRING:
        updatestring(file, indexdoc, indexelem, d, value);
        break;
    }
  fseek(file->F, oldpos, SEEK_SET);
}



bool updatestring(file* file, uint64_t indexdoc, uint64_t indexelem, doc *d, elem value) {

  uint64_t pos = offsetelem(d, indexelem);
  if (d->elemlist[indexelem].string_.len > value.string_.len)
  {
    fseek(file->F, pos, SEEK_SET);
    fwrite(&value.string_.len, sizeof(uint32_t), 1, file->F);
    fwrite(value.string_.data, sizeof(char), d->elemlist->string_.len, file->F);
  }
  else {
    fseek(file->F, file->header.endpos, SEEK_SET);
    free(d->elemlist[indexelem].string_.data);
    d->elemlist[indexdoc].string_.len = value.string_.len;
    d->elemlist[indexdoc].string_.data = value.string_.data;
    insert(file, d);
    delete(file, indexdoc);
  }

}





bool readoc(file* f, doc* d) {
  fread(&d->count, sizeof(uint64_t), 1, f->F);

  d->typelist = malloc(d->count * sizeof(elementType));
  d->elemlist = malloc(d->count * sizeof(elem));
  fread(d->typelist, sizeof(elementType), d->count, f->F);
  for ( uint64_t i = 0; i < d->count; ++i) {
    switch (d->typelist[i])
    {
      case TYPE_INT:
        fread(&d->elemlist[i].int_, sizeof(int32_t), 1, f->F);
        break;
      case TYPE_DOUBLE:
        fread(&d->elemlist[i].double_, sizeof(double), 1, f->F);
        break;
      case TYPE_BOOLEAN:
        fread(&d->elemlist[i].bool_, sizeof(uint8_t), 1, f->F);
        break;
      case TYPE_STRING:
        fread(&d->elemlist[i].string_.len, sizeof(uint32_t), 1, f->F);
        d->elemlist->string_.data = malloc(d->elemlist->string_.len);
        fread(d->elemlist[i].string_.data, sizeof(char), d->elemlist->string_.len, f->F);
        break;
    }
  }
}


uint64_t writedoc(file* file, doc* d) {

  // pos уже установлен
  uint64_t offset = 0;

  uint64_t pos = ftello(file->F);

  offset += fwrite(&d->count, sizeof(uint64_t), 1, file->F) *  sizeof(uint64_t); 
  offset += fwrite(d->typelist, sizeof(elem), d->count, file->F) * sizeof(elem);
  for (int64_t i = 0; i < d->count; ++i) {
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




bool getdoc(file* file, uint64_t index, doc* d) {

  uint64_t pos = ftello(file->F);
  if (file->list->list[index].status != OPEN) {
    fseek(file->F, file->list->list[index].pos, SEEK_SET);
    readoc(file, d);
    return true;
  }
  fseek(file->F, pos, SEEK_SET);
  return false;
}

void printdoc(doc* d) {
  printf("{\n");
  for (int64_t i = 0; i < d->count; ++i) {
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
  printf("\n{");
}



uint64_t calcdoc(doc* d) {
  uint64_t count = 0;
  count += sizeof(uint64_t);
  count += d->count * sizeof(elementType);
  for (int64_t i = 0; i < d->count; ++i) {
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
        count += sizeof(int32_t);
        count += d->elemlist[i].string_.len * sizeof(char);
        break;
    }
  }


}




