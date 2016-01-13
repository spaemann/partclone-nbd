/* Copyright © 2015-2016 Przemysław Kusiak
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef PARTCLONE_H_INCLUDED
#define PARTCLONE_H_INCLUDED

#include <inttypes.h>
#include <unistd.h>
#include <assert.h>

typedef enum {fatal = -2, error, ok} status;

/* short wrappers for GCC builtin functions */
#define swap16(x)       __builtin_bswap16(x)
#define swap32(x)       __builtin_bswap32(x)
#define swap64(x)       __builtin_bswap64(x)

/* why 8-bit integer? because on 64-bit machines it is the fastest algorithm */
#define popcount(x)     __builtin_popcountll(x)

/* shorter fixed types for storing integers */
typedef uint64_t    u64;
typedef int64_t     s64;
typedef uint32_t    u32;
typedef int32_t     s32;
typedef uint16_t    u16;
typedef int16_t     s16;
typedef uint8_t     u8;
typedef int8_t      s8;

/* fixed types for storing characters */
typedef int8_t      utf8;
typedef int16_t     utf16;
typedef int32_t     utf32;

/* format strings for printf */
#define fu8     "%"     PRIu8
#define fs8     "%"     PRId8
#define fu16    "%"     PRIu16
#define fs16    "%"     PRId16
#define fu32    "%"     PRIu32
#define fs32    "%"     PRId32
#define fu64    "%"     PRIu64
#define fs64    "%"     PRId64
#define fsize   "%zu"
#define fssize  "%zd"

/* useful macros */
#define kilobyte (1024)
#define megabyte (1024 * 1024)
#define gigabyte (1024 * 1024 * 1024)

#define divide_up(x, y) ((x) + ((y) - 1)) / (y)
#define divide_round(x, y) (((x) + 1) / (y))

/* NONE - all blocks are used (like dd tool) */
#define BITMAP_NONE 0x00
/* BIT - classical bitmap */
#define BITMAP_BIT  0x01
/* BYTE - each byte corresponds to one block (especially used in 0001 images) */
#define BITMAP_BYTE 0x08

#define ENDIANNESS_COMPATIBLE   0xC0DE
#define ENDIANNESS_INCOMPATIBLE 0xDEC0

/* check bit, if bit is set, macro return 1, else 0 */
#define CHECK_BIT(var,pos) (!(!((var) & (1<<(pos)))))

enum image_version {v1, v2};
enum checksum_mode {crc32, ignore};
enum bitmap_mode   {bit, byte, none};

struct image
{
    // ------------------------- BITMAP AND CACHE --------------------------

    // pointer to the first element of the bitmap
    u64 *bitmap_ptr;
    // size of the bitmap
    size_t bitmap_size;
    // bitmap elements (bitmap_size * 8)
    size_t bitmap_elements;
    // CRC32 or NONE
    enum bitmap_mode bmpmode;

    // number of blocks per one cache element
    u64 bitmap_elements_in_cache_element;
    // pointer to the bitmap cache
    u64 *cache_ptr;
    // size of the bitmap cache
    size_t cache_size;
    // cache elements (cache_size * 8)
    size_t cache_elements;

    // ---------------------------- PARAMETERS -----------------------------

    // file system path to an image
    char *path;
    // ENDIANNESS_COMPATIBLE (0xCODE) or ENDIANNESS_INCOMPATIBLE (0xDECO)
    u16 endianess_checker;
    // size of the source device in bytes
    u64 device_size;
    // number of blocks in the filesystem
    u64 blocks_count;
    // used blocks
	u64 used_blocks;
	// number of bytes in each block
    u32 block_size;
    // checksum algorithm used (CHECKSUM_CRC32 etc.)
	enum checksum_mode chkmode;
	// size of one checksum (ex. CHECKSUM_CRC32 - 4)
	u16 checksum_size;
	// how many blocks are checksumed together
	u32 blocks_per_checksum;
    // offset of data (from the beggining of the image)
    u64 data_offset;
    // offset of on-disk bitmap (from the beggining of the image)
    u64 bitmap_offset;
};

#endif