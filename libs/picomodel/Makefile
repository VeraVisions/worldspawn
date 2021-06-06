# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs 
DO_CC=$(CC) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.c.o:
	$(DO_CC)

WS_OBJS = \
	clip.o \
	envelope.o \
	list.o \
	lwio.o \
	lwo2.o \
	lwob.o \
	pntspols.o \
	surface.o \
	vecmath.o \
	vmap.o \
	picointernal.o \
	picomodel.o \
	picomodules.o \
	pm_3ds.o \
	pm_ase.o \
	pm_fm.o \
	pm_lwo.o \
	pm_md2.o \
	pm_md3.o \
	pm_mdc.o \
	pm_ms3d.o \
	pm_obj.o \
	pm_terrain.o \
	pm_iqm.o

# binary target
../libpicomodel.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
clip.o: clip.c
envelope.o: envelope.c
list.o: list.c
lwio.o: lwio.c
lwo2.o: lwo2.c lwo2.h
lwob.o: lwob.c
pntspols.o: pntspols.c
surface.o: surface.c
vecmath.o: vecmath.c
vmap.o: vmap.c

picointernal.o: picointernal.c picointernal.h
picomodel.o: picomodel.c ../picomodel.h
picomodules.o: picomodules.c
pm_3ds.o: pm_3ds.c
pm_ase.o: pm_ase.c
pm_fm.o: pm_fm.c pm_fm.h
pm_lwo.o: pm_lwo.c
pm_md2.o: pm_md2.c
pm_md3.o: pm_md3.c
pm_mdc.o: pm_mdc.c
pm_ms3d.o: pm_ms3d.c
pm_obj.o: pm_obj.c
pm_terrain.o: pm_terrain.c
pm_iqm.o: pm_iqm.c

clean:
	-rm -f *.o ../libpicomodel.a
