# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs 
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	callback.o constant.o object.o static.o

# binary target
../libgeneric.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
callback.o: callback.cpp callback.h
constant.o: constant.cpp constant.h
object.o: object.cpp object.h
static.o: static.cpp static.h

clean:
	-rm -f *.o ../libgeneric.a
