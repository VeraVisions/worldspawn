# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs -DPOSIX
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	signal.o

# binary target
../libsignal.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
signal.o: signal.cpp isignal.h signal.h signalfwd.h

clean:
	-rm -f *.o ../libsignal.a
