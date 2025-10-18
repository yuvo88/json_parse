#include "hashmap.h"

#include "parser.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main (int argc, const char** argv) {
    assert (argc == 2);
    const char* input = argv[1];
    ParseState state = {};
    state.buffer = input;
    state.position = 0;
    EntryValue* value = parseJson(&state);
    printEntryValue(value, 0);
    printf("\n");
    return 0;
}
