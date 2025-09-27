#include "superPrimitive.h"
#include <stdio.h>
#include <string.h>

int main () {
    SuperPrimitive* superInt = createSuperPrimitiveInt(10);
    SuperPrimitive* superFloat = createSuperPrimitiveFloat(10.10);
    SuperPrimitive* superChar = createSuperPrimitiveChar('a');
    SuperPrimitive* superBool = createSuperPrimitiveBool(1);
    const char* hello = "Hello World!";
    SuperPrimitive* superString = createSuperPrimitiveString(hello, strlen(hello));
    printSuperPrimitive(superInt);
    printf("hash is: %u\n", fnv1(superInt, 255));
    printSuperPrimitive(superFloat);
    printf("hash is: %u\n", fnv1(superFloat, 255));
    printSuperPrimitive(superChar);
    printf("hash is: %u\n", fnv1(superChar, 255));
    printSuperPrimitive(superBool);
    printSuperPrimitive(superString);
    printf("hash is: %u\n", fnv1(superString, 255));

    freeSuperPrimitive(superInt);
    freeSuperPrimitive(superFloat);
    freeSuperPrimitive(superChar);
    freeSuperPrimitive(superBool);
    freeSuperPrimitive(superString);
    return 0;
}
