#include "hashmap.h"
#include "superPrimitive.h"
#include <stdio.h>

int main () {
    Hashmap* hashmap       = createHashmap ();
    SuperPrimitive* key    = createSuperPrimitiveString ("O$!nM)LW", 8);
    SuperPrimitive* value  = createSuperPrimitiveString ("value", 5);
    SuperPrimitive* key2   = createSuperPrimitiveString ("{p)Q(A:u", 8);
    SuperPrimitive* value2 = createSuperPrimitiveString ("value2", 6);
    SuperPrimitive* key3   = createSuperPrimitiveString (").K4l/XR", 8);
    SuperPrimitive* value3 = createSuperPrimitiveString ("value2", 6);
    SuperPrimitive* key4   = createSuperPrimitiveString ("12345687", 8);
    SuperPrimitive* value4 = createSuperPrimitiveString ("value2", 6);
    setHashmapEntry (hashmap, key, value, SUPER_PRIMITIVE);
    setHashmapEntry (hashmap, key2, value2, SUPER_PRIMITIVE);
    setHashmapEntry (hashmap, key3, value3, SUPER_PRIMITIVE);
    setHashmapEntry (hashmap, key4, value4, SUPER_PRIMITIVE);

    printf("hashmap length: %u\n", hashmap->length);
    printHashmapln (hashmap);
    freeHashmap (hashmap);
    return 0;
}
