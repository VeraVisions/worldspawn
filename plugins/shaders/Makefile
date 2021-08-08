# WorldSpawn Makefile 

GLIB_CFLAGS=$(shell pkg-config --cflags gtk+-2.0) -DGTK_TARGET=2
GLIB_LDFLAGS=$(shell pkg-config --libs gtk+-2.0)

PLUGIN_CFLAGS=$(CFLAGS) $(GLIB_CFLAGS) -I../../include -I../../libs -fPIC -fvisibility=hidden
PLUGIN_LDFLAGS=$(LDFLAGS) $(GLIB_LDFLAGS) -shared

DO_CXX=$(CXX) $(PLUGIN_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	shaders.o plugin.o

# binary target
../../build/plugins/libshaders.so: $(WS_OBJS)
	$(CXX) -o $@ $(WS_OBJS) $(PLUGIN_LDFLAGS)

# object files
shaders.o: shaders.cpp shaders.h
plugin.o: plugin.cpp

clean:
	-rm -f *.o ../../build/plugins/libshaders.so
