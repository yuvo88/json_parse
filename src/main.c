#include "hashmap.h"
#include "superPrimitive.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main (int argc, const char** argv) {
    assert (argc == 2);
    const char* input = argv[1];
    List* list        = createList ();
    assert ((int)input[0] == 91);
    for (int i = 1; i < strlen (input); i++) {
        int ascii = (int)input[i];
        if (ascii >= 48 && ascii <= 57) { // check if input[i] is a digit
            int number = 0;
            while (ascii >= 48 && ascii <= 57) {
                number *= 10;
                number += ascii - 48;
                i++;
                ascii = input[i];
            }
            addValueToList (list,
            createEntryValue (createSuperPrimitiveInt (number), SUPER_PRIMITIVE));
        }
        assert (ascii == 44 || ascii == 93 || ascii == 32 ||
        ascii == 10); // check if input[i] is , ] whitespace \n
    }
    printListln (list);
    return 0;
}
