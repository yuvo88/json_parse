#ifndef __super_primitive_h
#define __super_primitive_h
#include <stdint.h>
typedef enum SuperPrimitiveType { INTEGER, FLOAT, STRING, BOOL, CHAR } SuperPrimitiveType;
typedef struct SuperPrimitive {
    void* value;
    uint32_t size;
    SuperPrimitiveType type;
} SuperPrimitive;
SuperPrimitive* createSuperPrimitiveInt(int value);
SuperPrimitive* createSuperPrimitiveFloat (float value);
SuperPrimitive* createSuperPrimitiveChar (char value);
SuperPrimitive* createSuperPrimitiveBool (uint8_t value);
SuperPrimitive* createSuperPrimitiveString (const char* value);
void printSuperPrimitive(SuperPrimitive* superPrimitive);
void freeSuperPrimitive(SuperPrimitive* superPrimitive);
uint32_t fnv1(SuperPrimitive* superPrimitive, uint32_t mask);
#endif
