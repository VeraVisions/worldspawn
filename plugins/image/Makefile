# WorldSpawn Makefile 

GLIB_CFLAGS=$(shell pkg-config --cflags gtk+-2.0) -DGTK_TARGET=2 -DXWINDOWS 
GLIB_LDFLAGS=$(shell pkg-config --libs gtk+-2.0)
PLUGIN_CFLAGS=$(CFLAGS) $(GLIB_CFLAGS) -I../../include -I../../libs -DPOSIX -fPIC -fvisibility=hidden
PLUGIN_LDFLAGS=$(LDFLAGS) $(GLIB_LDFLAGS) -shared -ljpeg

DO_CXX=$(CXX) $(PLUGIN_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	bmp.o dds.o image.o jpeg.o ktx.o pcx.o tga.o

# binary target
../../build/plugins/libimage.so: $(WS_OBJS)
	$(CXX) $(PLUGIN_LDFLAGS) -o $@ $(WS_OBJS) ../../libs/libddslib.a ../../libs/libetclib.a

# object files
bmp.o: bmp.cpp bmp.h
dds.o: dds.cpp dds.h
image.o: image.cpp
jpeg.o: jpeg.cpp jpeg.h
ktx.o: ktx.cpp ktx.h
pcx.o: pcx.cpp pcx.h
tga.o: tga.cpp tga.h

clean:
	-rm -f *.o ../../build/plugins/libimage.so
