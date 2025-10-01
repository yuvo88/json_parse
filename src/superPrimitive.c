#include "superPrimitive.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
SuperPrimitive* createSuperPrimitive (void* value, uint32_t size, SuperPrimitiveType type);
uint32_t fnv1 (SuperPrimitive* superPrimitive, uint32_t mask) {
    assert (superPrimitive->type != BOOL);
    uint32_t hash = 2166136261u;
    for (uint32_t i = 0; i < superPrimitive->size; i++) {
        hash *= 16777619u;
        hash ^= *((uint8_t*)superPrimitive->value + i);
    }

    return hash & mask;
}

SuperPrimitive* createSuperPrimitiveInt (int value) {
    int* valuePointer = malloc (sizeof (int));
    *valuePointer     = value;
    return createSuperPrimitive ((void*)valuePointer, sizeof (int), INTEGER);
}
SuperPrimitive* createSuperPrimitiveFloat (float value) {
    float* valuePointer = malloc (sizeof (float));
    *valuePointer       = value;
    return createSuperPrimitive ((void*)valuePointer, sizeof (float), FLOAT);
}

SuperPrimitive* createSuperPrimitiveChar (char value) {
    char* valuePointer = malloc (sizeof (char));
    *valuePointer      = value;
    return createSuperPrimitive ((void*)valuePointer, sizeof (char), CHAR);
}
SuperPrimitive* createSuperPrimitiveBool (uint8_t value) {
    uint8_t* valuePointer = malloc (sizeof (uint8_t));
    *valuePointer         = value;
    return createSuperPrimitive ((void*)valuePointer, sizeof (uint8_t), BOOL);
}
SuperPrimitive* createSuperPrimitiveString (const char* value, uint32_t length) {
    char* valuePointer = malloc (length * sizeof (char));
    strcpy (valuePointer, value);
    return createSuperPrimitive ((void*)valuePointer, sizeof (char) * length, STRING);
}

SuperPrimitive* createSuperPrimitive (void* value, uint32_t size, SuperPrimitiveType type) {
    SuperPrimitive* superPrimitive = malloc (sizeof (SuperPrimitive));
    superPrimitive->value          = value;
    superPrimitive->size           = size;
    superPrimitive->type           = type;
    return superPrimitive;
}

void printSuperPrimitive (SuperPrimitive* superPrimitive) {
    switch (superPrimitive->type) {
    case INTEGER: printf ("%d", *(int*)superPrimitive->value); break;
    case STRING: printf ("\"%s\"", (char*)superPrimitive->value); break;
    case FLOAT: printf ("%f", *(float*)superPrimitive->value); break;
    case BOOL:

        if (*(uint8_t*)superPrimitive->value) {
            printf ("true");
        } else {
            printf ("false");
        }
        break;
    case CHAR: printf ("%c", *(char*)superPrimitive->value); break;
    }
}

void freeSuperPrimitive (SuperPrimitive* superPrimitive) {
    free (superPrimitive->value);
    free (superPrimitive);
}
