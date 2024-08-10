
#ifndef EDITOR_API_H
#define EDITOR_API_H

#ifdef EDITOR_STATIC_DEFINE
#  define EDITOR_API
#  define EDITOR_NO_EXPORT
#else
#  ifndef EDITOR_API
#    ifdef Editor_EXPORTS
        /* We are building this library */
#      define EDITOR_API 
#    else
        /* We are using this library */
#      define EDITOR_API 
#    endif
#  endif

#  ifndef EDITOR_NO_EXPORT
#    define EDITOR_NO_EXPORT 
#  endif
#endif

#ifndef EDITOR_DEPRECATED
#  define EDITOR_DEPRECATED __declspec(deprecated)
#endif

#ifndef EDITOR_DEPRECATED_EXPORT
#  define EDITOR_DEPRECATED_EXPORT EDITOR_API EDITOR_DEPRECATED
#endif

#ifndef EDITOR_DEPRECATED_NO_EXPORT
#  define EDITOR_DEPRECATED_NO_EXPORT EDITOR_NO_EXPORT EDITOR_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef EDITOR_NO_DEPRECATED
#    define EDITOR_NO_DEPRECATED
#  endif
#endif

#endif /* EDITOR_API_H */
