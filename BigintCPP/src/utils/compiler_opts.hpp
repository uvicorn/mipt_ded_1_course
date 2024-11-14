#ifndef COMPILER_OPTS_H
#define COMPILER_OPTS_H




#if PLATFORM_IS_MSVC
    #define FORCE_INLINE __forceinline
#else
    #define FORCE_INLINE inline __attribute__((always_inline))
#endif

#define FORCE_OPTIMIZATION __attribute__((optimize("Ofast")))

#define PLATFORM_IS_WIN false
// #define PLATFORM_IS_MSVC (defined(_WIN32) || defined(_WIN64))
// WARNING: переписать это, сейчас не работает для винды

#endif
