# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs 
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	array.o hashtable.o

# binary target
../libcontainer.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
array.o: array.cpp array.h cache.h container.h  hashfunc.h
hashtable.o: hashtable.cpp hashtable.h stack.h

clean:
	-rm -f *.o ../libcontainer.a
