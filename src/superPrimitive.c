#include "superPrimitive.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

uint32_t fnv1(SuperPrimitive* superPrimitive, uint32_t mask) {
    // TODO: check that superPrimitive is not BOOL 
    uint32_t hash = 2166136261u;
    for (uint32_t i = 0; i < superPrimitive->size; i++) {
        hash *= 16777619u;
        hash ^= *((uint8_t*)superPrimitive->value + i);
    }

    return hash & mask;
}

SuperPrimitive* createSuperPrimitiveInt (int value) {
    int* valuePointer              = malloc (sizeof (int));
    *valuePointer                  = value;
    SuperPrimitive* superPrimitive = malloc (sizeof (SuperPrimitive));
    superPrimitive->value          = valuePointer;
    superPrimitive->size           = sizeof (int);
    superPrimitive->type           = INTEGER;
    return superPrimitive;
}
SuperPrimitive* createSuperPrimitiveFloat (float value) {
    float* valuePointer            = malloc (sizeof (float));
    *valuePointer                  = value;
    SuperPrimitive* superPrimitive = malloc (sizeof (SuperPrimitive));
    superPrimitive->value          = valuePointer;
    superPrimitive->size           = sizeof (float);
    superPrimitive->type           = FLOAT;
    return superPrimitive;
}

SuperPrimitive* createSuperPrimitiveChar (char value) {
    char* valuePointer            = malloc (sizeof (char));
    *valuePointer                  = value;
    SuperPrimitive* superPrimitive = malloc (sizeof (SuperPrimitive));
    superPrimitive->value          = valuePointer;
    superPrimitive->size           = sizeof (char);
    superPrimitive->type           = CHAR;
    return superPrimitive;
}
SuperPrimitive* createSuperPrimitiveBool (uint8_t value) {
    uint8_t* valuePointer            = malloc (sizeof (uint8_t));
    *valuePointer                  = value;
    SuperPrimitive* superPrimitive = malloc (sizeof (SuperPrimitive));
    superPrimitive->value          = valuePointer;
    superPrimitive->size           = sizeof (uint8_t);
    superPrimitive->type           = BOOL;
    return superPrimitive;
}
SuperPrimitive* createSuperPrimitiveString (const char* value) {
    u_int32_t length   = strlen (value);
    char* valuePointer = malloc (length * sizeof (char));
    strcpy (valuePointer, value);
    SuperPrimitive* superPrimitive = malloc (sizeof (SuperPrimitive));
    superPrimitive->value          = (void*)valuePointer;
    superPrimitive->size           = length * sizeof (char);
    superPrimitive->type           = STRING;
    return superPrimitive;
}
void printSuperPrimitive (SuperPrimitive* superPrimitive) {
    switch (superPrimitive->type) {
    case INTEGER: printf ("value: %d\n", *(int*)superPrimitive->value); break;
    case STRING: printf ("value: %s\n", (char*)superPrimitive->value); break;
    case FLOAT: printf ("value: %f\n", *(float*)superPrimitive->value); break;
    case BOOL:

        if (*(uint8_t*)superPrimitive->value) {
            printf ("value: true\n");
        } else {
            printf ("value: false\n");
        }
        break;
    case CHAR: printf ("value: %c\n", *(char*)superPrimitive->value); break;
    }
}

void freeSuperPrimitive (SuperPrimitive* superPrimitive) {
    free (superPrimitive->value);
    free (superPrimitive);
}
