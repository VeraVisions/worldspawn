set(CPACK_PACKAGE_NAME "WorldSpawn")
set(CPACK_PACKAGE_VERSION_MAJOR "${WorldSpawn_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${WorldSpawn_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${WorldSpawn_VERSION_PATCH}")

# binary: --target package
set(CPACK_GENERATOR "ZIP")
set(CPACK_STRIP_FILES 1)

# source: --target package_source
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_SOURCE_IGNORE_FILES "/\\\\.git/;/build/;/install/")

# configure
include(InstallRequiredSystemLibraries)
include(CPack)
