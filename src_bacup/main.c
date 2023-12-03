#include "filemanager.h"
#include "doc.h"
#include "opetarion.h"
#include <stdlib.h>
#include <sys/time.h>

int main()
{
  file *fi = createfile("test");
  // closefile(fi);
  // readfile(fi, "test");
  // printf("%u,%s\n", f->endpos, "endpos");

  // for (unsigned i = 0; i < 10; ++i) {
  //   printf("%i ", f->list->list[i].empty);
  //   printf("%d ", f->list->list[i].id);
  //   printf("%d ", f->list->list[i].parent);
  //   printf("%d ", f->list->list[i].pos);
  //   printf("%d \n", f->list->list[i].size);
  // }
  // int32_t a1 = 122.0;
  // elem a[5]= {4.4 , a1, 1.12341243, 1, 1};
  // elementType b[5]= {TYPE_DOUBLE,1,2,1,3};
  doc *d = createdoc(4);
  // string s = {.len = 150, .data = "mkfmkomrmvewokvmoewkmvokmwerokvmwokemvkowmevkomwvemk"};
  // addelem(d, 1, (elem)123);
  // addelem(d, 1, (elem)123L);
  // addelem(d, 4, (elem)s);
  // addelem(d, 4, (elem)s);
  addelem(d, TYPE_DOUBLE, (elem)12.341234);
  addelem(d, TYPE_DOUBLE, (elem)12.341234);
  addelem(d, TYPE_DOUBLE, (elem)12.341234);
  addelem(d, TYPE_DOUBLE, (elem)12.341234);

  // uint64_t old_size = f->endpos;
  // printf("%u,%s\n", f->endpos, "endpos");
  for (int i = 0; i < 1; ++i)
  {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    writedoc(fi, d, 0);
    gettimeofday(&end, NULL);

    printf("%d\t%lu\n", i, (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec);
  }

  inode node = getinode(fi, 0);

  //  for (int i = 0; i < 2; ++i)
  // {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    doc* di = readoc(fi, 0);
    printdoc(di);
    gettimeofday(&end, NULL);

    printf("%d\t%lu\n", 0, (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec);
  // }
  // for (int i = 0; i < 100000; ++i)
  // {
  //   struct timeval start, end;
  //   gettimeofday(&start, NULL);
  //   insert(f, d);
  //   gettimeofday(&end, NULL);

  //   printf("%d\t%lu\n", i, (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec);
  // }
  // printf("pos %d insert %d\n", f->endpos, f->endpos - old_size);

  // for (int i = 43; i > 5; --i)
  // old_size = f->endpos;
  // printf("", delete (f, 0));
  // {
  //   struct timeval start, end;
  //   gettimeofday(&start, NULL);
  //   printf("", delete (f, 1));
  //   gettimeofday(&end, NULL);
  //   printf("%d\t%lu\n", i, (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec);
  // }

  // printf("pos %d delete %d\n", f->endpos, f->endpos - old_size) ;

  // 
  // printdoc(d);
  // printf("%d ", sizeof(*d));

  // // printf("%u,%s\n", f->endpos   , "endpos");
  closefile(fi);
}
