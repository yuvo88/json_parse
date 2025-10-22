#include "hashmap.h"

#include "parser.h"
#include "readFile.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, const char** argv) {
    assert (argc == 2);
    const char* filename = argv[1];
    FileBuffer* buffer = openFile(filename, 4096);
    EntryValue* value = createEntryValue(NULL, SUPER_PRIMITIVE);
    uint32_t return_value = parseJson(buffer, value);
    if (!return_value) {
        printEntryValueln(value);
    }

    closeFile(buffer);
    return 0;
}
