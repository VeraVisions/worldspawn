# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs -DPOSIX
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	allocator.o

# binary target
../libmemory.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
allocator.o: allocator.cpp allocator.h

clean:
	-rm -f *.o ../libmemory.a
