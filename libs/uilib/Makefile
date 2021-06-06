# WorldSpawn Makefile 

GTK_CFLAGS=$(shell pkg-config --cflags gtk+-2.0)
LIB_CFLAGS=$(CFLAGS) $(GTK_CFLAGS) -I../../include -I../../libs -DGTK_TARGET=2 -DXWINDOWS
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	uilib.o

# binary target
../libuilib.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
uilib.o: uilib.cpp uilib.h

clean:
	-rm -f *.o ../libuilib.a
