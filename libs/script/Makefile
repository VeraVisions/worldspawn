# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs -DPOSIX
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
_.o 

# binary target
../libscript.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
_.o: _.cpp scripttokeniser.h scripttokenwriter.h

clean:
	-rm -f *.o ../libscript.a
