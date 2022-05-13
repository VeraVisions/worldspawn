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
	eclassmodel.o \
	entity.o \
	filters.o \
	generic.o \
	group.o \
	light.o \
	miscmodel.o \
	prop_dynamic.o \
	plugin.o \
	skincache.o \
	targetable.o

# binary target
../../build/plugins/libentity.$(LIB_EXT): $(WS_OBJS)
	$(CXX) -o $@ $(WS_OBJS) $(PLUGIN_LDFLAGS)

# object files
eclassmodel.o: eclassmodel.cpp eclassmodel.h
entity.o: entity.cpp entity.h
filters.o: filters.cpp filters.h
generic.o: generic.cpp generic.h
group.o: group.cpp group.h
light.o: light.cpp light.h
miscmodel.o: miscmodel.cpp miscmodel.h
prop_dynamic.o: prop_dynamic.cpp prop_dynamic.h
plugin.o: plugin.cpp
skincache.o: skincache.cpp skincache.h
targetable.o: targetable.cpp targetable.h

clean:
	-rm -f *.o ../../build/plugins/libentity.$(LIB_EXT)
