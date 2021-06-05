# vmap Makefile 

# ws libs vmap uses
LIBOBJS=../libs/libddslib.a \
        ../libs/libetclib.a \
        ../libs/libfilematch.a \
        ../libs/libl_net.a \
        ../libs/libmathlib.a \
        ../libs/libpicomodel.a

GLIB_CFLAGS=$(shell pkg-config --cflags glib-2.0)
GLIB_LDFLAGS=$(shell pkg-config --libs glib-2.0)
VMAP_CFLAGS=$(CFLAGS) $(GLIB_CFLAGS) -I../include  -I./common -I../libs -DPOSIX -Wno-narrowing
VMAP_LDFLAGS=$(LDFLAGS) $(GLIB_LDFLAGS) -lm -lpthread -L../lib -lxml2 -ljpeg -lpng -lminizip $(LIBOBJS)

DO_CC=$(CC) $(VMAP_CFLAGS) -o $@ -c $<

.c.o:
	$(DO_CC)

VMAP_OBJS = \
	common/cmdlib.o \
	common/imagelib.o \
	common/inout.o \
	common/jpeg.o \
	common/md4.o \
	common/mutex.o \
	common/polylib.o \
	common/scriplib.o \
	common/matlib.o \
	common/threads.o \
	common/vfs.o \
	vmap/brush.o \
	vmap/brush_primit.o \
	vmap/bsp.o \
	vmap/bsp_analyze.o \
	vmap/bsp_info.o \
	vmap/bsp_scale.o \
	vmap/bspfile_abstract.o \
	vmap/bspfile_ibsp.o \
	vmap/bspfile_rbsp.o \
	vmap/convert_ase.o \
	vmap/convert_bsp.o \
	vmap/convert_map.o \
	vmap/convert_obj.o \
	vmap/decals.o \
	vmap/exportents.o \
	vmap/facebsp.o \
	vmap/fixaas.o \
	vmap/fog.o \
	vmap/help.o \
	vmap/image.o \
	vmap/leakfile.o \
	vmap/light.o \
	vmap/light_bounce.o \
	vmap/light_trace.o \
	vmap/light_ydnar.o \
	vmap/lightmaps_ydnar.o \
	vmap/main.o \
	vmap/map.o \
	vmap/mesh.o \
	vmap/model.o \
	vmap/patch.o \
	vmap/path_init.o \
	vmap/portals.o \
	vmap/prtfile.o \
	vmap/shaders.o \
	vmap/surface.o \
	vmap/surface_extra.o \
	vmap/surface_foliage.o \
	vmap/surface_fur.o \
	vmap/surface_meta.o \
	vmap/tjunction.o \
	vmap/tree.o \
	vmap/vis.o \
	vmap/visflow.o \
	vmap/writebsp.o

# binary target
../build/vmap: $(VMAP_OBJS)
	$(CXX) -o $@ $(VMAP_OBJS) $(VMAP_LDFLAGS)

clean:
	-rm -f ./common/*.o
	-rm -f ./vmap/*.o
	-rm -f ../build/vmap

# object files
common/cmdlib.o: common/cmdlib.c common/cmdlib.h
common/imagelib.o: common/imagelib.c common/imagelib.h
common/inout.o: common/inout.c common/inout.h
common/jpeg.o: common/jpeg.c
common/md4.o: common/md4.c common/md4.h
common/mutex.o: common/mutex.c common/mutex.h
common/polylib.o: common/polylib.c common/polylib.h
common/scriplib.o: common/scriplib.c common/scriplib.h
common/matlib.o: common/matlib.c common/matlib.h
common/threads.o: common/threads.c
common/vfs.o: common/vfs.c common/vfs.h
vmap/brush.o: vmap/brush.c 
vmap/brush_primit.o: vmap/brush_primit.c
vmap/bsp.o: vmap/bsp.c
vmap/bsp_analyze.o: vmap/bsp_analyze.c
vmap/bsp_info.o: vmap/bsp_info.c
vmap/bsp_scale.o: vmap/bsp_scale.c
vmap/bspfile_abstract.o: vmap/bspfile_abstract.c
vmap/bspfile_ibsp.o: vmap/bspfile_ibsp.c
vmap/bspfile_rbsp.o: vmap/bspfile_rbsp.c
vmap/convert_ase.o: vmap/convert_ase.c
vmap/convert_bsp.o: vmap/convert_bsp.c
vmap/convert_map.o: vmap/convert_map.c
vmap/convert_obj.o: vmap/convert_obj.c
vmap/decals.o: vmap/decals.c
vmap/exportents.o: vmap/exportents.c
vmap/facebsp.o: vmap/facebsp.c
vmap/fixaas.o: vmap/fixaas.c
vmap/fog.o: vmap/fog.c
vmap/help.o: vmap/help.c
vmap/image.o: vmap/image.c
vmap/leakfile.o: vmap/leakfile.c
vmap/light.o: vmap/light.c
vmap/light_bounce.o: vmap/light_bounce.c
vmap/light_trace.o: vmap/light_trace.c
vmap/light_ydnar.o: vmap/light_ydnar.c
vmap/lightmaps_ydnar.o: vmap/lightmaps_ydnar.c
vmap/main.o: vmap/main.c
vmap/map.o: vmap/map.c
vmap/mesh.o: vmap/mesh.c
vmap/model.o: vmap/model.c
vmap/patch.o: vmap/patch.c
vmap/path_init.o: vmap/path_init.c
vmap/portals.o: vmap/portals.c
vmap/prtfile.o: vmap/prtfile.c
vmap/shaders.o: vmap/shaders.c
vmap/surface.o: vmap/surface.c
vmap/surface_extra.o: vmap/surface_extra.c
vmap/surface_foliage.o: vmap/surface_foliage.c
vmap/surface_fur.o: vmap/surface_fur.c
vmap/surface_meta.o: vmap/surface_meta.c
vmap/tjunction.o: vmap/tjunction.c
vmap/tree.o: vmap/tree.c
vmap/vis.o: vmap/vis.c
vmap/visflow.o: vmap/visflow.c
vmap/writebsp.o: vmap/writebsp.c
