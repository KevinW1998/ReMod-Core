
#ifndef REMOD_CORE_EXPORT_H
#define REMOD_CORE_EXPORT_H

#ifdef REMOD_CORE_STATIC_DEFINE
#  define REMOD_CORE_EXPORT
#  define REMOD_CORE_NO_EXPORT
#else
#  ifndef REMOD_CORE_EXPORT
#    ifdef ReModCore_EXPORTS
        /* We are building this library */
#      define REMOD_CORE_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define REMOD_CORE_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef REMOD_CORE_NO_EXPORT
#    define REMOD_CORE_NO_EXPORT 
#  endif
#endif

#ifndef REMOD_CORE_DEPRECATED
#  define REMOD_CORE_DEPRECATED __declspec(deprecated)
#endif

#ifndef REMOD_CORE_DEPRECATED_EXPORT
#  define REMOD_CORE_DEPRECATED_EXPORT REMOD_CORE_EXPORT REMOD_CORE_DEPRECATED
#endif

#ifndef REMOD_CORE_DEPRECATED_NO_EXPORT
#  define REMOD_CORE_DEPRECATED_NO_EXPORT REMOD_CORE_NO_EXPORT REMOD_CORE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef REMOD_CORE_NO_DEPRECATED
#    define REMOD_CORE_NO_DEPRECATED
#  endif
#endif

#endif /* REMOD_CORE_EXPORT_H */
