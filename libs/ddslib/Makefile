# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs -DPOSIX
DO_CC=$(CC) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.c.o:
	$(DO_CC)

WS_OBJS = \
	ddslib.o

# binary target
../libddslib.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
ddslib.o: ddslib.c

clean:
	-rm -f *.o ../libddslib.a
