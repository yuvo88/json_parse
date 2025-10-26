#ifndef __super_primitive_h
#define __super_primitive_h
#include "arena.h"
#include <stdint.h>
typedef enum SuperPrimitiveType {
    INTEGER,
    FLOAT,
    STRING,
    BOOL,
    CHAR
} SuperPrimitiveType;
typedef struct SuperPrimitive {
    void* value;
    uint32_t size;
    SuperPrimitiveType type;
} SuperPrimitive;
SuperPrimitive* createSuperPrimitiveInt (Arena* arena, int value);
SuperPrimitive* createSuperPrimitiveFloat (Arena* arena, float value);
SuperPrimitive* createSuperPrimitiveChar (Arena* arena, char value);
SuperPrimitive* createSuperPrimitiveBool (Arena* arena, uint8_t value);
SuperPrimitive* createSuperPrimitiveString (Arena* arena, const char* value, uint32_t length);
void printSuperPrimitive (SuperPrimitive* superPrimitive);
void freeSuperPrimitive (SuperPrimitive* superPrimitive);
uint32_t fnv1 (SuperPrimitive* superPrimitive);
#endif
