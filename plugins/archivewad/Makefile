# WorldSpawn Plugin Makefile 

PLUGIN_CFLAGS=$(CFLAGS) -I../../include -I../../libs -fPIC -fvisibility=hidden
PLUGIN_LDFLAGS=$(LDFLAGS) -shared
LIB_EXT=so

DO_CXX=$(CXX) $(PLUGIN_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	archive.o plugin.o

# binary target
../../build/plugins/libarchivewad.$(LIB_EXT): $(WS_OBJS)
	$(CXX) -o $@ $(WS_OBJS) $(PLUGIN_LDFLAGS)

# object files
archive.o: archive.cpp archive.h
plugin.o: plugin.cpp

clean:
	-rm -f *.o ../../build/plugins/libarchivewad.$(LIB_EXT)
