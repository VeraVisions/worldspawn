# WorldSpawn Plugin Makefile 

GLIB_CFLAGS=$(shell pkg-config --cflags gtk+-2.0) -DGTK_TARGET=2 -DXWINDOWS 
GLIB_LDFLAGS=$(shell pkg-config --libs gtk+-2.0)
PLUGIN_CFLAGS=$(CFLAGS) $(GLIB_CFLAGS) -I../../include -I../../libs -DPOSIX -fPIC -fvisibility=hidden -DWorldSpawn_VERSION=1 -DWorldSpawn_MAJOR_VERSION=1 -DWorldSpawn_MINOR_VERSION=0 -DWorldSpawn_PATCH_VERSION=0
PLUGIN_LDFLAGS=$(LDFLAGS) $(GLIB_LDFLAGS) -shared

DO_CXX=$(CXX) $(PLUGIN_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	AboutDialog.o ConfigDialog.o LoadPortalFileDialog.o portals.o prtview.o

# binary target
../../build/plugins/libprtview.so: $(WS_OBJS)
	$(CXX) $(PLUGIN_LDFLAGS) -o $@ $(WS_OBJS) ../../libs/libuilib.a ../../libs/libgtkutil.a ../../libs/libprofile.a

# object files
AboutDialog.o: AboutDialog.cpp AboutDialog.h
ConfigDialog.o: ConfigDialog.cpp ConfigDialog.h
LoadPortalFileDialog.o: LoadPortalFileDialog.cpp LoadPortalFileDialog.h
portals.o: portals.cpp portals.h
prtview.o: prtview.cpp prtview.h

clean:
	-rm -f *.o ../../build/plugins/libprtview.so