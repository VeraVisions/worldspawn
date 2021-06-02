# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs -DPOSIX
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	debugging.o

# binary target
../libdebugging.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
debugging.o: debugging.cpp debugging.h

clean:
	-rm -f *.o ../libdebugging.a
