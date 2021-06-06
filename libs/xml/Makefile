# WorldSpawn Makefile

GTK_CFLAGS=$(shell pkg-config --cflags gtk+-2.0)
LIB_CFLAGS=$(CFLAGS) $(GTK_CFLAGS) -I../../include -I../../libs -DGTK_TARGET=2 
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	xmltextags.o

# binary target
../libxmllib.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
xmltextags.o: ixml.h xmlelement.h xmlparser.h xmltextags.cpp xmltextags.h xmlwriter.h

clean:
	-rm -f *.o ../libxmllib.a
