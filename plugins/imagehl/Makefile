# WorldSpawn Plugin Makefile 

GLIB_CFLAGS=$(shell pkg-config --cflags gtk+-2.0) -DGTK_TARGET=2
GLIB_LDFLAGS=$(shell pkg-config --libs gtk+-2.0)

PLUGIN_CFLAGS=$(CFLAGS) $(GLIB_CFLAGS) -I../../include -I../../libs -fPIC -fvisibility=hidden
PLUGIN_LDFLAGS=$(LDFLAGS) $(GLIB_LDFLAGS) -shared

DO_CXX=$(CXX) $(PLUGIN_CFLAGS) $(SHLIBCFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	hlw.o imagehl.o mip.o sprite.o

# binary target
../../build/plugins/libimagehl.so: $(WS_OBJS)
	$(CXX) -o $@ $(WS_OBJS) ../../libs/libuilib.a ../../libs/libgtkutil.a $(PLUGIN_LDFLAGS)

# object files
hlw.o: hlw.cpp hlw.h
imagehl.o: imagehl.cpp
mip.o: mip.cpp mip.h
sprite.o: sprite.cpp sprite.h

clean:
	-rm -f *.o ../../build/plugins/libimagehl.so
