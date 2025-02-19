#ifndef FILE_OPS_H
#define FILE_OPS_H

#include <stddef.h>

// Read a file into memory using memory mapping
// Returns a pointer to the mapped content, or NULL on error
// If size_out is not NULL, writes the file size to it
char* read_file(const char* path, size_t* size_out);

// Close a memory-mapped file
void close_file(char* content);

#endif // FILE_OPS_H