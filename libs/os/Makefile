# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs 
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
_.o 

# binary target
../libos.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
_.o: _.cpp dir.h file.h path.h

clean:
	-rm -f *.o ../libos.a
