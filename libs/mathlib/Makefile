# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs 
DO_CC=$(CC) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.c.o:
	$(DO_CC)

WS_OBJS = \
	bbox.o line.o m4x4.o mathlib.o ray.o

# binary target
../libmathlib.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
bbox.o: bbox.c
line.o: line.c
m4x4.o: m4x4.c
mathlib.o: mathlib.c ../mathlib.h
ray.o: ray.c

clean:
	-rm -f *.o ../libmathlib.a
