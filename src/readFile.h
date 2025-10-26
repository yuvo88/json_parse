#ifndef __read_file_h
#define __read_file_h
#include "arena.h"
#include <stdint.h>
#include <stdio.h>
typedef struct FileBuffer {
    FILE* fd;
    uint32_t absolutePosition;
    uint32_t cursorPosition;
    uint32_t bufferSize;
    uint8_t* byteBuffer;
} FileBuffer;

FileBuffer* openFile (Arena* arena, const char* filename, uint32_t bufferSize);
void closeFile (FileBuffer* buffer);
uint32_t addToPosition (FileBuffer* buffer, uint32_t amount);
uint32_t getPosition (FileBuffer* buffer);
uint8_t getValue (FileBuffer* buffer, int64_t offset);
int isEndOfFile (FileBuffer* buffer);
int isEndOfFileAmount (FileBuffer* buffer, uint32_t amount);

#endif
