//------------------------------------------------------------------------------
// Contains a collection of data handling utilities and types.
//------------------------------------------------------------------------------

#pragma once

#include <stdint.h>

//------------------------------------------------------------------------------
// Basic types and definitions
//------------------------------------------------------------------------------

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;

typedef float     f32;
typedef double    f64;

typedef uintptr_t usize;

#define KB(x) ((x) * 1024)
#define MB(x) (KB(x) * 1024)
#define GB(x) (MB(x) * 1024)

//------------------------------------------------------------------------------
// Low-level memory management functions
//------------------------------------------------------------------------------

void* mem_reserve(usize size);
void  mem_release(void* ptr);

//------------------------------------------------------------------------------
// Arena memory management
//------------------------------------------------------------------------------

typedef struct
{
    u8* base;
    u8* end;
    u64 cursor;
} Arena;

// Arena management functions
void  arena_init(Arena* arena, usize size);
void  arena_done(Arena* arena);
void* arena_alloc(Arena* arena, usize size);

#define ARENA_ALLOC(arena, type) ( type* )arena_alloc((arena), sizeof(type))
