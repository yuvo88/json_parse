#include "hashmap.h"

#include "parser.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main (int argc, const char** argv) {
    assert (argc == 2);
    const char* input = argv[1];
    ParseState state  = {};
    state.buffer      = input;
    state.position    = 0;
    EntryValue* value = createEntryValue(NULL, 0);
    uint32_t position = parseJson (&state, value);
    if (position != 0) {
        printf ("There was an error in position: %u\n", position);
        return 0;
    }
    printEntryValue (value, 0);
    printf ("\n");
    return 0;
}
