#ifndef __parser_h
#define __parser_h
#include "hashmap.h"
#include "readFile.h"
#include <assert.h>
#include <stdint.h>
#include "arena.h"
typedef struct ParseState {
    const char* buffer;
    uint32_t position;
}ParseState ;
uint32_t parseJson (Arena* arena, FileBuffer* buffer, EntryValue* parsedReturn);
#endif
