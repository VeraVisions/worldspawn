# WorldSpawn Plugin Makefile 

GLIB_CFLAGS=$(shell pkg-config --cflags gtk+-2.0) -DGTK_TARGET=2
GLIB_LDFLAGS=$(shell pkg-config --libs gtk+-2.0)

PLUGIN_CFLAGS=$(CFLAGS) $(GLIB_CFLAGS) -I../../include -I../../libs -fPIC -fvisibility=hidden
PLUGIN_LDFLAGS=$(LDFLAGS) $(GLIB_LDFLAGS) -shared
LIB_EXT=so

DO_CXX=$(CXX) $(PLUGIN_CFLAGS) $(SHLIBCFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	callbacks.o export.o interface.o plugin.o support.o

# binary target
../../build/plugins/libbrushexport.$(LIB_EXT): $(WS_OBJS)
	$(CXX) -o $@ $(WS_OBJS) ../../libs/libuilib.a ../../libs/libgtkutil.a $(PLUGIN_LDFLAGS)

# object files
callbacks.o: callbacks.cpp callbacks.h
export.o: export.cpp export.h
interface.o: interface.cpp
plugin.o: plugin.cpp plugin.h
support.o: support.cpp support.h

clean:
	-rm -f *.o ../../build/plugins/libbrushexport.$(LIB_EXT)
