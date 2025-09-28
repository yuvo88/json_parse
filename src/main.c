#include "hashmap.h"

int main () {
    Hashmap* hashmap = createHashmap ();
    freeHashmap (hashmap);
    return 0;
}
