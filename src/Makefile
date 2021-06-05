# WorldSpawn Makefile 

GTK_CFLAGS=$(shell pkg-config --cflags gtk+-2.0)
GTK_LDFLAGS=$(shell pkg-config --libs gtk+-2.0)

WS_LIBS= ../libs/libcmdlib.a \
../libs/libcontainer.a \
../libs/libddslib.a \
../libs/libdebugging.a \
../libs/libetclib.a \
../libs/libgeneric.a \
../libs/libgtkutil.a \
../libs/libl_net.a \
../libs/libmath.a \
../libs/libmathlib.a \
../libs/libmodulesystem.a \
../libs/libos.a \
../libs/libpicomodel.a \
../libs/libprofile.a \
../libs/libscript.a \
../libs/libsignal.a \
../libs/libsplines.a \
../libs/libstream.a \
../libs/libstring.a \
../libs/libuilib.a \
../libs/libxmllib.a

WS_CFLAGS=$(CFLAGS) $(GTK_CFLAGS) -I../include -I../libs -DGTK_TARGET=2 $(WS_VERSION) -DPOSIX -DXWINDOWS
WS_LDFLAGS=$(LDFLAGS) -lm $(GTK_LDFLAGS) -lX11 -lGL -lxml2 -lgdkglext-x11-1.0 -lgtkglext-x11-1.0 -L../lib $(WS_LIBS)

DO_CXX=$(CXX) $(WS_CFLAGS) -o $@ -c $<

.cpp.o:
	$(DO_CXX)

WS_OBJS = \
	autosave.o \
	brush.o \
	brush_primit.o \
	brushmanip.o \
	brushmodule.o \
	brushnode.o \
	brushtokens.o \
	brushxml.o \
	build.o \
	camwindow.o \
	clippertool.o \
	commands.o \
	console.o \
	csg.o \
	dialog.o \
	eclass.o \
	eclass_def.o \
	eclass_doom3.o \
	eclass_fgd.o \
	eclass_xml.o \
	entity.o \
	entityinspector.o \
	entitylist.o \
	environment.o \
	error.o \
	feedback.o \
	filetypes.o \
	filters.o \
	findtexturedialog.o \
	glwidget.o \
	grid.o \
	groupdialog.o \
	gtkdlgs.o \
	gtkmisc.o \
	help.o \
	image.o \
	main.o \
	mainframe.o \
	map.o \
	mru.o \
	nullmodel.o \
	parse.o \
	patch.o \
	patchdialog.o \
	patchmanip.o \
	patchmodule.o \
	plugin.o \
	pluginapi.o \
	pluginmanager.o \
	pluginmenu.o \
	plugintoolbar.o \
	points.o \
	preferencedictionary.o \
	preferences.o \
	qe3.o \
	qgl.o \
	referencecache.o \
	renderer.o \
	renderstate.o \
	scenegraph.o \
	select.o \
	selection.o \
	server.o \
	shaders.o \
	sockets.o \
	stacktrace.o \
	surfacedialog.o \
	texmanip.o \
	textureentry.o \
	textures.o \
	texwindow.o \
	timer.o \
	treemodel.o \
	undo.o \
	url.o \
	view.o \
	watchbsp.o \
	winding.o \
	windowobservers.o \
	xmlstuff.o \
	xywindow.o

# binary target
../build/worldspawn: $(WS_OBJS)
	$(CXX) -o $@ $(WS_OBJS) $(WS_LDFLAGS)

clean:
	-rm -f *.o ../build/worldspawn

# object files
autosave.o: autosave.cpp autosave.h
brush.o: brush.cpp brush.h
brush_primit.o: brush_primit.cpp brush_primit.h
brushmanip.o: brushmanip.cpp brushmanip.h
brushmodule.o: brushmodule.cpp brushmodule.h
brushnode.o: brushnode.cpp brushnode.h
brushtokens.o: brushtokens.cpp brushtokens.h
brushxml.o: brushxml.cpp brushxml.h
build.o: build.cpp build.h
camwindow.o: camwindow.cpp camwindow.h
clippertool.o: clippertool.cpp clippertool.h
commands.o: commands.cpp commands.h
console.o: console.cpp console.h
csg.o: csg.cpp csg.h
dialog.o: dialog.cpp dialog.h
eclass.o: eclass.cpp eclass.h
eclass_def.o: eclass_def.cpp eclass_def.h
eclass_doom3.o: eclass_doom3.cpp eclass_doom3.h
eclass_fgd.o: eclass_fgd.cpp eclass_fgd.h
eclass_xml.o: eclass_xml.cpp eclass_xml.h
entity.o: entity.cpp entity.h
entityinspector.o: entityinspector.cpp entityinspector.h
entitylist.o: entitylist.cpp entitylist.h
environment.o: environment.cpp environment.h
error.o: error.cpp error.h
feedback.o: feedback.cpp feedback.h
filetypes.o: filetypes.cpp filetypes.h
filters.o: filters.cpp filters.h
findtexturedialog.o: findtexturedialog.cpp findtexturedialog.h
glwidget.o: glwidget.cpp glwidget.h
grid.o: grid.cpp grid.h
groupdialog.o: groupdialog.cpp groupdialog.h
gtkdlgs.o: gtkdlgs.cpp gtkdlgs.h
gtkmisc.o: gtkmisc.cpp gtkmisc.h
help.o: help.cpp help.h
image.o: image.cpp image.h
main.o: main.cpp main.h
mainframe.o: mainframe.cpp mainframe.h
map.o: map.cpp map.h
mru.o: mru.cpp mru.h
nullmodel.o: nullmodel.cpp nullmodel.h
parse.o: parse.cpp parse.h
patch.o: patch.cpp patch.h
patchdialog.o: patchdialog.cpp patchdialog.h
patchmanip.o: patchmanip.cpp patchmanip.h
patchmodule.o: patchmodule.cpp patchmodule.h
plugin.o: plugin.cpp plugin.h
pluginapi.o: pluginapi.cpp pluginapi.h
pluginmanager.o: pluginmanager.cpp pluginmanager.h
pluginmenu.o: pluginmenu.cpp pluginmenu.h
plugintoolbar.o: plugintoolbar.cpp plugintoolbar.h
points.o: points.cpp points.h
preferencedictionary.o: preferencedictionary.cpp preferencedictionary.h
preferences.o: preferences.cpp preferences.h
qe3.o: qe3.cpp qe3.h
qgl.o: qgl.cpp qgl.h
referencecache.o: referencecache.cpp referencecache.h
renderer.o: renderer.cpp renderer.h
renderstate.o: renderstate.cpp renderstate.h
scenegraph.o: scenegraph.cpp scenegraph.h
select.o: select.cpp select.h
selection.o: selection.cpp selection.h
server.o: server.cpp server.h
shaders.o: shaders.cpp shaders.h
sockets.o: sockets.cpp sockets.h
stacktrace.o: stacktrace.cpp stacktrace.h
surfacedialog.o: surfacedialog.cpp surfacedialog.h
texmanip.o: texmanip.cpp texmanip.h
textureentry.o: textureentry.cpp textureentry.h
textures.o: textures.cpp textures.h
texwindow.o: texwindow.cpp texwindow.h
timer.o: timer.cpp timer.h
treemodel.o: treemodel.cpp treemodel.h
undo.o: undo.cpp undo.h
url.o: url.cpp url.h
view.o: view.cpp view.h
watchbsp.o: watchbsp.cpp watchbsp.h
winding.o: winding.cpp winding.h
windowobservers.o: windowobservers.cpp windowobservers.h
xmlstuff.o: xmlstuff.cpp xmlstuff.h
xywindow.o: xywindow.cpp xywindow.h
