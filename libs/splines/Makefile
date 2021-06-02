# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs -DPOSIX
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	math_angles.o \
	math_matrix.o \
	math_quaternion.o \
	math_vector.o \
	q_parse.o \
	q_shared.o \
	splines.o \
	util_str.o

# binary target
../libsplines.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
math_angles.o: math_angles.cpp math_angles.h
math_matrix.o: math_matrix.cpp math_matrix.h
math_quaternion.o: math_quaternion.cpp math_quaternion.h
math_vector.o: math_vector.cpp math_vector.h
q_parse.o: q_parse.cpp
q_shared.o: q_shared.cpp q_shared.h
splines.o: splines.cpp splines.h
util_str.o: util_str.cpp util_str.h util_list.h

clean:
	-rm -f *.o ../libsplines.a
