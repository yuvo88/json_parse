#include "hashmap.h"

#include "parser.h"
#include <assert.h>
#include <stdint.h>

int main (int argc, const char** argv) {
    assert (argc == 2);
    const char* input = argv[1];
    ParseState state = {};
    state.buffer = input;
    state.position = 0;
    Hashmap* hashmap = parseHashmap(&state);
    printHashmapln(hashmap);
    return 0;
}
