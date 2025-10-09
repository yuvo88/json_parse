#ifndef __parser_h
#define __parser_h
#include "hashmap.h"
#include <assert.h>
#include <stdint.h>
typedef struct ParseState {
    const char* buffer;
    uint32_t position;
}ParseState ;
List* parseList (ParseState* state);
#endif
