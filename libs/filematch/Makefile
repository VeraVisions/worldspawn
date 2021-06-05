# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs -DPOSIX
DO_CC=$(CC) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.c.o:
	$(DO_CC)

WS_OBJS = \
	filematch.o

# binary target
../libfilematch.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
filematch.o: filematch.c ../filematch.h

clean:
	-rm -f *.o ../libfilematch.a
