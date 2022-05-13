# WorldSpawn Makefile 

GLIB_CFLAGS=$(shell pkg-config --cflags gtk+-2.0) -DGTK_TARGET=2
GLIB_LDFLAGS=$(shell pkg-config --libs gtk+-2.0)

JPEG_CFLAGS=$(shell pkg-config --cflags libjpeg)
JPEG_LDFLAGS=$(shell pkg-config --libs libjpeg)

PLUGIN_CFLAGS=$(CFLAGS) $(GLIB_CFLAGS) $(JPEG_CFLAGS) -I../../include -I../../libs -fPIC -fvisibility=hidden
PLUGIN_LDFLAGS=$(LDFLAGS) $(GLIB_LDFLAGS) $(JPEG_LDFLAGS) -shared
LIB_EXT=so

DO_CXX=$(CXX) $(PLUGIN_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	bmp.o dds.o image.o jpeg.o ktx.o pcx.o tga.o

# binary target
../../build/plugins/libimage.$(LIB_EXT): $(WS_OBJS)
	$(CXX) -o $@ $(WS_OBJS) ../../libs/libddslib.a ../../libs/libetclib.a $(PLUGIN_LDFLAGS)

# object files
bmp.o: bmp.cpp bmp.h
dds.o: dds.cpp dds.h
image.o: image.cpp
jpeg.o: jpeg.cpp jpeg.h
ktx.o: ktx.cpp ktx.h
pcx.o: pcx.cpp pcx.h
tga.o: tga.cpp tga.h

clean:
	-rm -f *.o ../../build/plugins/libimage.$(LIB_EXT)
