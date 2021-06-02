# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs -DPOSIX
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	_.o

# binary target
../libstream.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
_.o: _.cpp filestream.h memstream.h stringstream.h textfilestream.h textstream.h

clean:
	-rm -f *.o ../libstream.a
