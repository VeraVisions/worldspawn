#ifndef INCLUDED_LIBS_GLOBALDEFS
#define INCLUDED_LIBS_GLOBALDEFS

// ARCH_ENDIAN

#if defined(__BIG_ENDIAN__) || defined(_SGI_SOURCE)
#define GDEF_ARCH_ENDIAN_BIG 1
#else
#define GDEF_ARCH_ENDIAN_BIG 0
#endif

// ARCH_BITS

#if defined(__i386__) || defined(_M_IX86)
#define GDEF_ARCH_BITS_32 1
#else
#define GDEF_ARCH_BITS_32 0
#endif

#if defined(__LP64__) || defined(_M_X64) || defined(_M_AMD64) || defined(_WIN64)
#define GDEF_ARCH_BITS_64 1
#else
#define GDEF_ARCH_BITS_64 0
#endif

// OS

#if defined(__linux__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__)
#define GDEF_OS_POSIX 1
#define XWINDOWS 1
#else
#define GDEF_OS_POSIX 0
#define XWINDOWS 0
#endif

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define GDEF_OS_WINDOWS 1
#else
#define GDEF_OS_WINDOWS 0
#endif

#if defined(__APPLE__)
#define GDEF_OS_MACOS 1
#else
#define GDEF_OS_MACOS 0
#endif

#if defined(__linux__)
#define GDEF_OS_LINUX 1
#else
#define GDEF_OS_LINUX 0
#endif


#define GDEF_OS_BSD 0

#if defined(__FreeBSD__)
#undef GDEF_OS_BSD
#define GDEF_OS_BSD 1
#define GDEF_OS_BSD_FREE 1
#else
#define GDEF_OS_BSD_FREE 0
#endif

#if defined(__NetBSD__)
#undef GDEF_OS_BSD
#define GDEF_OS_BSD 1
#define GDEF_OS_BSD_NET 1
#else
#define GDEF_OS_BSD_NET 0
#endif

#if defined(__OpenBSD__)
#undef GDEF_OS_BSD
#define GDEF_OS_BSD 1
#define GDEF_OS_BSD_OPEN 1
#else
#define GDEF_OS_BSD_OPEN 0
#endif

#if defined(__DragonFly__)
#undef GDEF_OS_BSD
#define GDEF_OS_BSD 1
#define GDEF_OS_BSD_DRAGONFLY 1
#else
#define GDEF_OS_BSD_DRAGONFLY 0
#endif

// COMPILER

#if defined(_MSC_VER)
#define GDEF_COMPILER_MSVC 1
#else
#define GDEF_COMPILER_MSVC 0
#endif

#if defined(__GNUC__)
#define GDEF_COMPILER_GNU 1
#else
#define GDEF_COMPILER_GNU 0
#endif

// ATTRIBUTE

#if GDEF_COMPILER_GNU
#define GDEF_ATTRIBUTE_NORETURN __attribute__((noreturn))
#else
#define GDEF_ATTRIBUTE_NORETURN
#endif

#ifdef GDEF_COMPILER_MSVC
#define GDEF_ATTRIBUTE_INLINE __inline
#else
#define GDEF_ATTRIBUTE_INLINE inline
#endif

// MISC

#define GDEF_DEBUG 0
#if defined(_DEBUG)
#if _DEBUG
#undef GDEF_DEBUG
#define GDEF_DEBUG 1
#endif
#endif

#endif
