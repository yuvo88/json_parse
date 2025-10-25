#include "hashmap.h"

#include "parser.h"
#include "readFile.h"
#include "superPrimitive.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
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
    printf("%lu\n", sizeof(HashmapEntry) + sizeof(SuperPrimitive) * 2 + sizeof(EntryValue));

    closeFile(buffer);
    return 0;
}
