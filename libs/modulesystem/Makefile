# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs -DPOSIX
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	singletonmodule.o

# binary target
../libmodulesystem.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
singletonmodule.o: singletonmodule.cpp singletonmodule.h moduleregistry.h modulesmap.h

clean:
	-rm -f *.o ../libmodulesystem.a
