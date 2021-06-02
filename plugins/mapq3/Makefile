# WorldSpawn Makefile 

GLIB_CFLAGS=$(shell pkg-config --cflags gtk+-2.0) -DGTK_TARGET=2 -DXWINDOWS 
GLIB_LDFLAGS=$(shell pkg-config --libs gtk+-2.0)
PLUGIN_CFLAGS=$(CFLAGS) $(GLIB_CFLAGS) -I../../include -I../../libs -DPOSIX -fPIC -fvisibility=hidden
PLUGIN_LDFLAGS=$(LDFLAGS) $(GLIB_LDFLAGS) -shared

DO_CXX=$(CXX) $(PLUGIN_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	parse.o plugin.o write.o

# binary target
../../build/plugins/libmapq3.so: $(WS_OBJS)
	$(CXX) $(PLUGIN_LDFLAGS) -o $@ $(WS_OBJS)


# object files
parse.o: parse.cpp parse.h
plugin.o: plugin.cpp
write.o: write.cpp write.h

clean:
	-rm -f *.o ../../build/plugins/libmapq3.so
