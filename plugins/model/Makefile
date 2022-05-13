# WorldSpawn Makefile 

GLIB_CFLAGS=$(shell pkg-config --cflags gtk+-2.0) -DGTK_TARGET=2
GLIB_LDFLAGS=$(shell pkg-config --libs gtk+-2.0)

PLUGIN_CFLAGS=$(CFLAGS) $(GLIB_CFLAGS) -I../../include -I../../libs -fPIC -fvisibility=hidden
PLUGIN_LDFLAGS=$(LDFLAGS) $(GLIB_LDFLAGS) -shared
LIB_EXT=so

DO_CXX=$(CXX) $(PLUGIN_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	model.o plugin.o

# binary target
../../build/plugins/libmodel.$(LIB_EXT): $(WS_OBJS)
	$(CXX) -o $@ $(WS_OBJS) ../../libs/libpicomodel.a $(PLUGIN_LDFLAGS)

# object files
model.o: model.cpp model.h
plugin.o: plugin.cpp

clean:
	-rm -f *.o ../../build/plugins/libmodel.$(LIB_EXT)