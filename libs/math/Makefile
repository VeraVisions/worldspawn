# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs 
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
_.o 

# binary target
../libmath.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
_.o: _.cpp aabb.h curve.h frustum.h line.h matrix.h pi.h plane.h quaternion.h vector.h

clean:
	-rm -f *.o ../libmath.a
