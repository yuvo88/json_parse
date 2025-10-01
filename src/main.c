#include "hashmap.h"
#include "superPrimitive.h"
#include <stdio.h>

int main () {
    Hashmap* hashmap       = createHashmap ();
    SuperPrimitive* key    = createSuperPrimitiveString ("key", 3);
    SuperPrimitive* value  = createSuperPrimitiveString ("value", 5);
    setKeyAndValue (hashmap, key, value, SUPER_PRIMITIVE);

    Hashmap* pHashmap       = createHashmap ();
    SuperPrimitive* pKey    = createSuperPrimitiveString ("key2", 4);
    SuperPrimitive* pValue  = createSuperPrimitiveString ("value", 5);
    setKeyAndValue (pHashmap, pKey, pValue, SUPER_PRIMITIVE);

    Hashmap* yHashmap       = createHashmap ();
    SuperPrimitive* yKey    = createSuperPrimitiveString ("key2", 4);
    SuperPrimitive* yValue  = createSuperPrimitiveString ("value", 5);
    setKeyAndValue (yHashmap, yKey, yValue, SUPER_PRIMITIVE);


    SuperPrimitive* bKey    = createSuperPrimitiveString ("key2", 4);
    setKeyAndValue(pHashmap, bKey, yHashmap, HASHMAP); 

    SuperPrimitive* tKey    = createSuperPrimitiveString ("key2", 4);
    setKeyAndValue(hashmap, tKey, pHashmap, HASHMAP); 
    printHashmap (hashmap);

    freeHashmap (hashmap);
    return 0;
}
