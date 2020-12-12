set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
include_directories(BEFORE common)

set(Q3MAP_VERSION 2.5.17n)
find_package(Git REQUIRED)
execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
if (GIT_VERSION)
    set(Q3MAP_VERSION "${Q3MAP_VERSION}-git-${GIT_VERSION}")
endif ()
add_definitions(-DQ3MAP_VERSION="${Q3MAP_VERSION}")

find_package(GLIB REQUIRED)
include_directories(${GLIB_INCLUDE_DIRS})

find_package(JPEG REQUIRED)
include_directories(${JPEG_INCLUDE_DIR})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})

#find_package(Minizip REQUIRED)
#include_directories(${Minizip_INCLUDE_DIRS})

set(vmap_games
        vmap/game_fte.h
        )

radiant_tool(vmap
        common/cmdlib.c common/cmdlib.h
        common/imagelib.c common/imagelib.h
        common/inout.c common/inout.h
        common/jpeg.c
        common/md4.c common/md4.h
        common/mutex.c common/mutex.h
        common/polylib.c common/polylib.h
        common/polyset.h
        common/qfiles.h
        common/qthreads.h
        common/scriplib.c common/scriplib.h
        common/matlib.c common/matlib.h
        common/surfaceflags.h
        common/threads.c
        common/vfs.c common/vfs.h
        vmap/brush.c
        vmap/brush_primit.c
        vmap/bsp.c
        vmap/bsp_analyze.c
        vmap/bsp_info.c
        vmap/bsp_scale.c
        vmap/bspfile_abstract.c
        vmap/bspfile_ibsp.c
        vmap/bspfile_rbsp.c
        vmap/convert_ase.c
        vmap/convert_bsp.c
        vmap/convert_map.c
        vmap/convert_obj.c
        vmap/decals.c
        vmap/exportents.c
        vmap/facebsp.c
        vmap/fixaas.c
        vmap/fog.c
        ${vmap_games} vmap/game__null.h
        vmap/help.c
        vmap/image.c
        vmap/leakfile.c
        vmap/light.c
        vmap/light_bounce.c
        vmap/light_trace.c
        vmap/light_ydnar.c
        vmap/lightmaps_ydnar.c
        vmap/main.c
        vmap/map.c
        vmap/mesh.c
        vmap/model.c
        vmap/patch.c
        vmap/path_init.c
        vmap/portals.c
        vmap/prtfile.c
        vmap/vmap.h
        vmap/shaders.c
        vmap/surface.c
        vmap/surface_extra.c
        vmap/surface_foliage.c
        vmap/surface_fur.c
        vmap/surface_meta.c
        vmap/tjunction.c
        vmap/tree.c
        vmap/vis.c
        vmap/visflow.c
        vmap/writebsp.c
        )

target_link_libraries(vmap
        ${GLIB_LIBRARIES}
        ${JPEG_LIBRARIES}
        ${PNG_LIBRARIES}
        ${LIBXML2_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ddslib
        etclib
        filematch
        l_net
        mathlib
        minizip
        picomodel
        )

if (UNIX)
    target_link_libraries(vmap pthread m)
endif ()
