#include "hashmap.h"
#include "superPrimitive.h"

int main () {
    List* list = createList ();
    addValueToList (list,
    createEntryValue (createSuperPrimitiveString ("123", 3), SUPER_PRIMITIVE));
    addValueToList (list, createEntryValue (createSuperPrimitiveInt (1), SUPER_PRIMITIVE));
    printListln (list);

    return 0;
}
