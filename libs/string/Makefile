# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs 
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	pooledstring.o

# binary target
../libstring.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
pooledstring.o: pooledstring.cpp pooledstring.h string.h stringfwd.h

clean:
	-rm -f *.o ../libstring.a
