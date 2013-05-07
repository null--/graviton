#ifndef _LZLIB_H_
#define _LZLIB_H_

#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lauxlib.h"

#include "zlib.h"

/*
** =========================================================================
** zlib stream metamethods
** =========================================================================
*/
#define ZSTREAMMETA "zlib:zstream"

#define LZ_ANY     -1
#define LZ_NONE    0
#define LZ_DEFLATE 1
#define LZ_INFLATE 2

#if 0
    #define LZ_BUFFER_SIZE LUAL_BUFFERSIZE
#else
    #define LZ_BUFFER_SIZE 8192
#endif

typedef struct {
    /* zlib structures */
    z_stream zstream;
    /* stream state. LZ_DEFLATE | LZ_INFLATE */
    int state;
    int error;
    int peek;
    int eos;
    /* user callback source for reading/writing */
    int io_cb;
    /* input buffer */
    int i_buffer_ref;
    size_t i_buffer_pos;
    size_t i_buffer_len;
    const char *i_buffer;
    /* output buffer */
    size_t o_buffer_len;
    size_t o_buffer_max;
    char o_buffer[LZ_BUFFER_SIZE];
    /* dictionary */
    const Bytef *dictionary;
    size_t dictionary_len;
} lz_stream;

LUALIB_API int luaopen_zlib(lua_State *L);

#endif
