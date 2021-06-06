# WorldSpawn Makefile 

GLIB_CFLAGS=$(shell pkg-config --cflags gtk+-2.0) -DGTK_TARGET=2  
GLIB_LDFLAGS=$(shell pkg-config --libs gtk+-2.0)
PLUGIN_CFLAGS=$(CFLAGS) $(GLIB_CFLAGS) -I../../include -I../../libs -fPIC -fvisibility=hidden
PLUGIN_LDFLAGS=$(LDFLAGS) $(GLIB_LDFLAGS) -shared

DO_CXX=$(CXX) $(PLUGIN_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	archive.o vfs.o vfspk3.o

# binary target
../../build/plugins/libvfspk3.so: $(WS_OBJS)
	$(CXX) $(PLUGIN_LDFLAGS) -o $@ $(WS_OBJS) ../../libs/libfilematch.a


# object files
archive.o: archive.cpp archive.h
vfs.o: vfs.cpp vfs.h
vfspk3.o: vfspk3.cpp

clean:
	-rm -f *.o ../../build/plugins/libvfspk3.so
