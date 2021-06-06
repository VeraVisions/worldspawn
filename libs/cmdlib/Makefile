# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs 
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	cmdlib.o

# binary target
../libcmdlib.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
cmdlib.o: cmdlib.cpp

clean:
	-rm -f *.o ../libcmdlib.a
