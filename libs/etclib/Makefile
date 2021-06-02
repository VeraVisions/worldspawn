# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs -DPOSIX
DO_CC=$(CC) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.c.o:
	$(DO_CC)

WS_OBJS = \
	etclib.o

# binary target
../libetclib.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
etclib.o: etclib.c etclib.h

clean:
	-rm -f *.o ../libetclib.a
