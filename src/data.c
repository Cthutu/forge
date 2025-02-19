#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "data.h"

//------------------------------------------------------------------------------
// Low-level memory management functions
//------------------------------------------------------------------------------

void* mem_reserve(usize size)
{
    void* ptr =
        VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if ( !ptr ) {
        fprintf(stderr, "Failed to allocate memory: %lu\n", GetLastError());
        exit(1);
    }
    return ptr;
}

void mem_release(void* ptr)
{
    if ( ptr ) {
        VirtualFree(ptr, 0, MEM_RELEASE);
    }
}

//------------------------------------------------------------------------------
// Arena memory management
//------------------------------------------------------------------------------

void arena_init(Arena* arena, usize size)
{
    arena->base = ( u8* )mem_reserve(size);
    if ( !arena->base ) {
        return;
    }
    arena->end    = arena->base + size;
    arena->cursor = 0;
}

void arena_done(Arena* arena)
{
    if ( arena->base ) {
        mem_release(arena->base);
        arena->base   = NULL;
        arena->end    = NULL;
        arena->cursor = 0;
    }
}

void* arena_alloc(Arena* arena, usize size)
{
    if ( !arena->base ) {
        return NULL;
    }

    // Align size to 8 bytes for better memory access
    size = (size + 7) & ~7;

    if ( arena->cursor + size > ( usize )(arena->end - arena->base) ) {
        return NULL; // Not enough space
    }

    void* ptr      = arena->base + arena->cursor;
    arena->cursor += size;
    return ptr;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
