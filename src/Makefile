CC= gcc
CFLAGS= -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64



all: clear doc



doc: doc.c doc.h
	$(CC) -c $(CFLAGS) doc.c

listnode: listnode.c listnode.h
	$(CC) -c $(CFLAGS) listnode.c


inode: inode.c inode.h
	$(CC) -c $(CFLAGS) inode.c

irootnode: irootnode.c irootnode.h
	$(CC) -c $(CFLAGS) irootnode.c

clear:
	rm -f *.h.gch *.o




