#include "superPrimitive.h"
#include <stdio.h>

int main () {
    SuperPrimitive* superInt = createSuperPrimitiveInt(10);
    SuperPrimitive* superFloat = createSuperPrimitiveFloat(10.10);
    SuperPrimitive* superChar = createSuperPrimitiveChar('a');
    SuperPrimitive* superBool = createSuperPrimitiveBool(1);
    SuperPrimitive* superString = createSuperPrimitiveString("Hello World!");
    printSuperPrimitive(superInt);
    printf("hash is: %u\n", fnv1(superInt, 255));
    printSuperPrimitive(superFloat);
    printf("hash is: %u\n", fnv1(superFloat, 255));
    printSuperPrimitive(superChar);
    printf("hash is: %u\n", fnv1(superChar, 255));
    printSuperPrimitive(superBool);
    printf("hash is: %u\n", fnv1(superBool, 255));
    printSuperPrimitive(superString);
    printf("hash is: %u\n", fnv1(superString, 255));

    freeSuperPrimitive(superInt);
    freeSuperPrimitive(superFloat);
    freeSuperPrimitive(superChar);
    freeSuperPrimitive(superBool);
    freeSuperPrimitive(superString);
    return 0;
}
