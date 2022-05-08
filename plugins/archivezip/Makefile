# WorldSpawn Plugin Makefile 

ZLIB_LDFLAGS=$(shell pkg-config --libs zlib)

PLUGIN_CFLAGS=$(CFLAGS) -I../../include -I../../libs -fPIC -fvisibility=hidden
PLUGIN_LDFLAGS=$(LDFLAGS) -shared $(ZLIB_LDFLAGS)

DO_CXX=$(CXX) $(PLUGIN_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	archive.o plugin.o

# binary target
../../build/plugins/libarchivezip.so: $(WS_OBJS)
	$(CXX) -o $@ $(WS_OBJS) $(PLUGIN_LDFLAGS)

# object files
archive.o: archive.cpp archive.h
plugin.o: plugin.cpp zlibstream.h

clean:
	-rm -f *.o ../../build/plugins/libarchivezip.so
