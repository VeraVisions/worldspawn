# WorldSpawn Makefile 

GTK_CFLAGS=$(shell pkg-config --cflags gtk+-2.0)
LIB_CFLAGS=$(CFLAGS) $(GTK_CFLAGS) -I../../include -I/usr/include/gtkglext-1.0 -I/usr/lib64/gtkglext-1.0/include/ -I../../libs -DGTK_TARGET=2 -DPOSIX -DXWINDOWS
DO_CXX=$(CXX) -static -fPIC $(LIB_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
accelerator.o \
button.o \
clipboard.o \
cursor.o \
dialog.o \
entry.o \
filechooser.o \
frame.o \
glfont.o \
glwidget.o \
image.o \
menu.o \
messagebox.o \
nonmodal.o \
paned.o \
toolbar.o \
widget.o \
window.o \
xorrectangle.o \

# binary target
../libgtkutil.a: $(WS_OBJS)
	ar rcs $@ $(WS_OBJS)

# object files
accelerator.o: accelerator.cpp accelerator.h
button.o: button.cpp button.h
clipboard.o: clipboard.cpp clipboard.h
cursor.o: cursor.cpp cursor.h
dialog.o: dialog.cpp dialog.h
entry.o: entry.cpp entry.h
filechooser.o: filechooser.cpp filechooser.h
frame.o: frame.cpp frame.h
glfont.o: glfont.cpp glfont.h
glwidget.o: glwidget.cpp glwidget.h
image.o: image.cpp image.h
menu.o: menu.cpp menu.h
messagebox.o: messagebox.cpp messagebox.h
nonmodal.o: nonmodal.cpp nonmodal.h
paned.o: paned.cpp paned.h
toolbar.o: toolbar.cpp toolbar.h
widget.o: widget.cpp widget.h
window.o: window.cpp window.h
xorrectangle.o: xorrectangle.cpp xorrectangle.h

clean:
	-rm -f *.o ../libgtkutil.a
