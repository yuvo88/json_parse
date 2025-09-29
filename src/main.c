#include "hashmap.h"
#include "superPrimitive.h"
#include <stdio.h>

int main () {
    Hashmap* hashmap       = createHashmap ();
    SuperPrimitive* key    = createSuperPrimitiveString ("key", 3);
    SuperPrimitive* key2   = createSuperPrimitiveString ("key2", 4);
    SuperPrimitive* value  = createSuperPrimitiveString ("value", 5);
    SuperPrimitive* value2 = createSuperPrimitiveString ("value2", 6);
    setKeyAndValue (hashmap, key, value, value->size, SUPER_PRIMITIVE);
    setKeyAndValue (hashmap, key2, value2, value->size, SUPER_PRIMITIVE);
    for (int i = 0; i < hashmap->length; i++) {
        if (hashmap->entries[i] != NULL) {
            printSuperPrimitive (hashmap->entries[i]->key);
            printSuperPrimitive (hashmap->entries[i]->value);
        }
    }
    freeHashmap (hashmap);
    return 0;
}
