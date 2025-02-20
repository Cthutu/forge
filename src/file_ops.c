#include "forge.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

char* read_file(const char* path, size_t* size_out)
{
    HANDLE file = CreateFileA(path,
                              GENERIC_READ,
                              FILE_SHARE_READ,
                              NULL,
                              OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);

    if ( file == INVALID_HANDLE_VALUE ) {
        fprintf(stderr, "Error opening file '%s': %lu\n", path, GetLastError());
        return NULL;
    }

    LARGE_INTEGER file_size;
    if ( !GetFileSizeEx(file, &file_size) ) {
        fprintf(stderr, "Error getting file size: %lu\n", GetLastError());
        CloseHandle(file);
        return NULL;
    }

    if ( file_size.QuadPart > SIZE_MAX ) {
        fprintf(stderr, "File too large to map\n");
        CloseHandle(file);
        return NULL;
    }

    HANDLE mapping = CreateFileMappingA(file, NULL, PAGE_READONLY, 0, 0, NULL);

    if ( !mapping ) {
        fprintf(stderr, "Error creating file mapping: %lu\n", GetLastError());
        CloseHandle(file);
        return NULL;
    }

    void* mapped_view = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);

    if ( !mapped_view ) {
        fprintf(stderr, "Error mapping view of file: %lu\n", GetLastError());
        CloseHandle(mapping);
        CloseHandle(file);
        return NULL;
    }

    // We don't need the handles anymore after mapping
    CloseHandle(mapping);
    CloseHandle(file);

    if ( size_out ) {
        *size_out = ( size_t )file_size.QuadPart;
    }

    return ( char* )mapped_view;
}

void close_file(char* content) { UnmapViewOfFile(content); }
