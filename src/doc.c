#include "doc.h"
#include "filemanager.h"


//  todo:
//  не хватате вывода в консоль






bool addelem (doc* d, elementType type, elem elem) {
  uint64_t i = 0;
  for (;d->typelist[i] != UNDEFINE && i < d->count; ++i);

  if (d->count == i) {
    d->count <<= 1;
    d->elemlist = realloc(d->elemlist, d->count);
    d->typelist = realloc(d->typelist, d->count);
  }; // тут пересоздание

  d->typelist[i] = type;
  d->elemlist[i] = elem;
}



doc* createdoc(uint64_t count) {
  doc* d = malloc(sizeof(d));
  d->count = count;
  d->typelist = calloc(count, sizeof(elementType));
  for (uint64_t i = 0; i < count; ++i) {
    d->typelist[i] = UNDEFINE;
  }
  d->elemlist = calloc(count, sizeof(elem));
  return d;
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


uint64_t writedoc(file* f, doc* d) {

  // pos уже установлен
  uint64_t offset = 0;

  uint64_t pos = ftello(f->F);
  offset += fwrite(d->typelist, sizeof(elem), d->count, f->F);
  for (int64_t i = 0; i < d->count; ++i) {
    switch (d->typelist[i]) {
      case TYPE_INT:
        offset += fwrite(&d->elemlist[i].int_, sizeof(int32_t), 1, f->F);
        break;
      case TYPE_DOUBLE:
        offset += fwrite(&d->elemlist[i].double_, sizeof(double), 1, f->F);
        break;
      case TYPE_BOOLEAN:
        offset += fwrite(&d->elemlist[i].bool_, sizeof(uint8_t), 1, f->F);
        break;
      case TYPE_STRING:
        offset += fwrite(&d->elemlist[i].string_.len, sizeof(uint32_t), 1, f->F);
        offset += fwrite(d->elemlist[i].string_.data, sizeof(char), d->elemlist->string_.len, f->F);
        break;
    }
  }
  fseek(f->F, pos, SEEK_SET);
  return offset;
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



uint64_t countdoc(doc* d) {
  uint64_t count = 0;
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




