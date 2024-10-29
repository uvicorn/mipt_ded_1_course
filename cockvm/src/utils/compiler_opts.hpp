#ifndef COMPILER_OPTS_H
#define COMPILER_OPTS_H


#if PLATFORM_IS_MSVC
    #define FORCE_INLINE __forceinline
#else
    #define FORCE_INLINE inline __attribute__((always_inline))
#endif


#endif
