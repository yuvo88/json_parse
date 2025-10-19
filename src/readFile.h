#ifndef __read_file_h
#define __read_file_h
#include <stdint.h>
typedef struct FileBuffer {
    int fd;
    uint32_t position;
    uint8_t* buffer;
} FileBuffer;

FileBuffer* openFile(char* filename);
void closeFile (FileBuffer* buffer);
uint32_t addToPosition(FileBuffer* buffer, uint32_t amount);
uint32_t getPosition(FileBuffer* buffer);
uint8_t getCurrentValue(FileBuffer* buffer);

#endif
