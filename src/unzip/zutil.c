/* zutil.c -- target dependent utility functions for the compression library
 * Copyright (C) 1995-2017 Jean-loup Gailly
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/* @(#) $Id$ */

#include "zutil.h"
#ifndef Z_SOLO
#  include "gzguts.h"
#endif

z_const char * const z_errmsg[10] = {
    (z_const char *)"need dictionary",     /* Z_NEED_DICT       2  */
    (z_const char *)"stream end",          /* Z_STREAM_END      1  */
    (z_const char *)"",                    /* Z_OK              0  */
    (z_const char *)"file error",          /* Z_ERRNO         (-1) */
    (z_const char *)"stream error",        /* Z_STREAM_ERROR  (-2) */
    (z_const char *)"data error",          /* Z_DATA_ERROR    (-3) */
    (z_const char *)"insufficient memory", /* Z_MEM_ERROR     (-4) */
    (z_const char *)"buffer error",        /* Z_BUF_ERROR     (-5) */
    (z_const char *)"incompatible version",/* Z_VERSION_ERROR (-6) */
    (z_const char *)""
};


const char * ZEXPORT zlibVersion()
{
    return ZLIB_VERSION;
}

uLong ZEXPORT zlibCompileFlags()
{
    uLong flags;

    flags = 0;
    switch ((int)(sizeof(uInt))) {
    case 2:     break;
    case 4:     flags += 1;     break;
    case 8:     flags += 2;     break;
    default:    flags += 3;
    }
    switch ((int)(sizeof(uLong))) {
    case 2:     break;
    case 4:     flags += 1 << 2;        break;
    case 8:     flags += 2 << 2;        break;
    default:    flags += 3 << 2;
    }
    switch ((int)(sizeof(voidpf))) {
    case 2:     break;
    case 4:     flags += 1 << 4;        break;
    case 8:     flags += 2 << 4;        break;
    default:    flags += 3 << 4;
    }
    switch ((int)(sizeof(z_off_t))) {
    case 2:     break;
    case 4:     flags += 1 << 6;        break;
    case 8:     flags += 2 << 6;        break;
    default:    flags += 3 << 6;
    }
#if defined(STDC) || defined(Z_HAVE_STDARG_H)
#  ifdef NO_vsnprintf
    flags += 1L << 25;
#    ifdef HAS_vsprintf_void
    flags += 1L << 26;
#    endif
#  else
#    ifdef HAS_vsnprintf_void
    flags += 1L << 26;
#    endif
#  endif
#endif
    return flags;
}


/* exported to allow conversion of error code to string for compress() and
 * uncompress()
 */
const char * ZEXPORT zError(err)
    int err;
{
    return ERR_MSG(err);
}


#ifndef Z_SOLO

#ifndef MY_ZCALLOC /* Any system without a special alloc function */

voidpf ZLIB_INTERNAL zcalloc (opaque, items, size)
    voidpf opaque;
    unsigned items;
    unsigned size;
{
    (void)opaque;
 //   return sizeof(uInt) > 2 ? (voidpf)malloc(items * size) :
 //                             (voidpf)calloc(items, size);
    return NULL;
}

void ZLIB_INTERNAL zcfree (opaque, ptr)
    voidpf opaque;
    voidpf ptr;
{
    (void)opaque;
//    free(ptr);
}

#endif /* MY_ZCALLOC */

#endif /* !Z_SOLO */