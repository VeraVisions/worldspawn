# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs -DPOSIX
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	file.o profile.o

# binary target
../libprofile.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
file.o: file.cpp file.h
profile.o: profile.cpp profile.h

clean:
	-rm -f *.o ../libprofile.a
