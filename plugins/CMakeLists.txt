set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/modules")

add_custom_target(modules)
macro(radiant_plugin name)
    message(STATUS "Found Module ${name}")
    add_library(${name} MODULE ${ARGN})
    add_dependencies(modules ${name})
    copy_dlls(${name})
    install(
            TARGETS ${name}
            LIBRARY DESTINATION modules
    )
endmacro()

add_subdirectory(archivezip)
add_subdirectory(entity)
add_subdirectory(image)
add_subdirectory(mapq3)
add_subdirectory(iqmmodel)
add_subdirectory(model)
add_subdirectory(shaders)
add_subdirectory(vfspk3)
