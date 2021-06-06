# WorldSpawn Makefile 

LIB_CFLAGS=$(CFLAGS) -I../../include -I../../libs 
DO_CC=$(CC) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.c.o:
	$(DO_CC)

WS_OBJS = \
	l_net.o l_net_berkley.o

# binary target
../libl_net.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
l_net.o: l_net.c l_net.h
l_net_berkley.o: l_net_berkley.c

clean:
	-rm -f *.o ../libl_net.a
