#include "readFile.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FileBuffer* openFile (const char* filename, uint32_t bufferSize) {
    FILE* fd = fopen (filename, "r");
    if (fd == NULL) {
        return NULL;
    }

    uint8_t* byteBuffer = (uint8_t*)(malloc (bufferSize));
    uint32_t itemsRead  = fread (byteBuffer, 1, bufferSize, fd);
    if (itemsRead == 0) {
        return NULL;
    }

    FileBuffer* buffer       = (FileBuffer*)(malloc (sizeof (FileBuffer)));
    buffer->byteBuffer       = byteBuffer;
    buffer->bufferSize       = bufferSize;
    buffer->fd               = fd;
    buffer->absolutePosition = 0;
    buffer->cursorPosition   = itemsRead;

    return buffer;
}

void closeFile (FileBuffer* buffer) {
    fclose (buffer->fd);
    free (buffer->byteBuffer);
    free (buffer);
}

uint32_t addToPosition (FileBuffer* buffer, uint32_t amount) {
    if (buffer->absolutePosition + amount > buffer->cursorPosition) {
        uint32_t itemsRead =
        fread (buffer->byteBuffer, 1, buffer->bufferSize, buffer->fd);
        buffer->cursorPosition += itemsRead;
    }
    buffer->absolutePosition += amount;

    return buffer->absolutePosition;
}

uint32_t getPosition (FileBuffer* buffer) {
    return buffer->absolutePosition;
}

uint8_t getValue (FileBuffer* buffer, int64_t offset) {
    return buffer->byteBuffer[(buffer->absolutePosition % buffer->bufferSize) + offset];
}

int isEndOfFile (FileBuffer* buffer) {
    return isEndOfFileAmount (buffer, 0);
}
int isEndOfFileAmount (FileBuffer* buffer, uint32_t amount) {
    if (buffer->absolutePosition + amount >= buffer->cursorPosition &&
    buffer->cursorPosition % buffer->bufferSize != 0) {
        return 1;
    }
    return 0;
}
