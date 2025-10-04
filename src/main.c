#include "hashmap.h"
#include "superPrimitive.h"
#include <stdio.h>

int main () {
    Hashmap* hashmap       = createHashmap ();
    SuperPrimitive* key    = createSuperPrimitiveString ("key", 3);
    SuperPrimitive* value  = createSuperPrimitiveString ("value", 5);
    SuperPrimitive* key2   = createSuperPrimitiveString ("key2", 4);
    SuperPrimitive* value2 = createSuperPrimitiveString ("value2", 6);
    SuperPrimitive* key3   = createSuperPrimitiveString ("key3", 4);
    SuperPrimitive* value3 = createSuperPrimitiveString ("value3", 6);
    setKeyAndValue (hashmap, key, value, SUPER_PRIMITIVE);
    setKeyAndValue (hashmap, key2, value, SUPER_PRIMITIVE);



    printHashmap (hashmap);
    freeHashmap (hashmap);
    return 0;
}
